/*
 * Copyright (C) 2018 Microchip Technology Inc.  All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "egt/detail/basicwindow.h"
#include "egt/detail/planewindow.h"
#include "egt/painter.h"
#include "egt/kmsscreen.h"
#include "egt/window.h"
#include <algorithm>
#include <vector>
#include <iostream>
#include <map>

using namespace std;

namespace egt
{
inline namespace v1
{

static std::vector<Window*> the_windows;
static Window* the_main_window = nullptr;
static Window* the_modal_window = nullptr;

Window*& main_window()
{
    return the_main_window;
}

Window*& modal_window()
{
    return the_modal_window;
}

std::vector<Window*>& windows()
{
    return the_windows;
}

std::ostream& operator<<(std::ostream& os, const windowhint& event)
{
    static std::map<windowhint, std::string> strings;
    if (strings.empty())
    {
#define MAPITEM(p) strings[p] = #p
        MAPITEM(windowhint::automatic);
        MAPITEM(windowhint::software);
        MAPITEM(windowhint::overlay);
        MAPITEM(windowhint::heo_overlay);
        MAPITEM(windowhint::cursor_overlay);
#undef MAPITEM
    }

    os << strings[event];
    return os;
}

Window::Window(const Rect& rect,
               const widgetflags& flags,
               pixel_format format,
               windowhint hint)
    : Frame(rect, flags)
{
    set_name("Window" + std::to_string(m_widgetid));

    set_flag(widgetflag::WINDOW);
    set_boxtype(Theme::boxtype::fillsolid);

    // by default, windows are hidden
    set_flag(widgetflag::INVISIBLE);

    // create the window implementation
    create_impl(box(), format, hint);

    // save off the new window to the window list
    windows().push_back(this);
}

void Window::do_draw()
{
    if (unlikely(m_damage.empty()))
        return;

    DBG(name() << " " << __PRETTY_FUNCTION__);

    Painter painter(screen()->context());

    for (auto& damage : m_damage)
    {
        draw(painter, damage);
    }

    screen()->flip(m_damage);
    m_damage.clear();
}

void Window::resize(const Size& size)
{
    // cannot resize if we are screen
    if (unlikely(the_main_window == this))
        return;

    if (m_impl)
        m_impl->resize(size);
}

void Window::create_impl(const Rect& rect,
                         pixel_format format,
                         windowhint hint)
{
    ignoreparam(format);
    ignoreparam(hint);

    assert(main_screen());

    if (!the_main_window)
    {
        the_main_window = this;
        m_box = main_screen()->box();
        m_impl.reset(new detail::BasicTopWindow(this));
    }
    else
    {
        m_box = rect;

        try
        {
            switch (hint)
            {
            case windowhint::software:
                m_impl.reset(new detail::BasicWindow(this));
                break;
            case windowhint::overlay:
            case windowhint::heo_overlay:
            case windowhint::cursor_overlay:
#ifdef HAVE_LIBPLANES
                m_impl.reset(new detail::PlaneWindow(this, format, hint));
                set_flag(widgetflag::PLANE_WINDOW);
#endif
                break;
            default:
                break;
            }
        }
        catch (std::exception& e)
        {
            DBG(e.what());
        }

        if (!m_impl)
        {
#ifdef HAVE_LIBPLANES
            try
            {
                m_impl.reset(new detail::PlaneWindow(this, format, hint));
                set_flag(widgetflag::PLANE_WINDOW);
            }
            catch (std::exception& e)
            {
                DBG(e.what());
#endif

                m_impl.reset(new detail::BasicWindow(this));
#ifdef HAVE_LIBPLANES
            }
#endif
        }
    }

    assert(m_impl);

    if (is_flag_set(widgetflag::PLANE_WINDOW))
    {
        DBG(name() << " backend is PlaneWindow");
    }
    else
    {
        DBG(name() << " backend is BasicWindow");
    }
}

void Window::set_main_window()
{
    the_main_window = this;

    assert(main_screen());

    m_box = main_screen()->box();

    damage();
}

Window::~Window()
{
    auto i = find(the_windows.begin(), the_windows.end(), this);
    if (i != the_windows.end())
    {
        the_windows.erase(i);
    }

    if (the_main_window == this)
    {
        the_main_window = nullptr;

        for (auto& window : the_windows)
        {
            window->set_main_window();
            break;
        }
    }
}

}
}
