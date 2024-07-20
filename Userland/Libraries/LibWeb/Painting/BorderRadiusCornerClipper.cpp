/*
 * Copyright (c) 2022, MacDue <macdue@dueutil.tech>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <LibWeb/Painting/BorderRadiusCornerClipper.h>
#include <LibWeb/Painting/PaintContext.h>

namespace Web::Painting {

ScopedCornerRadiusClip::ScopedCornerRadiusClip(PaintContext& context, DevicePixelRect const& border_rect, BorderRadiiData const& border_radii, CornerClip corner_clip)
    : m_context(context)
    , m_id(context.allocate_corner_clipper_id())
    , m_border_rect(border_rect)
{
    CornerRadii const corner_radii {
        .top_left = border_radii.top_left.as_corner(context),
        .top_right = border_radii.top_right.as_corner(context),
        .bottom_right = border_radii.bottom_right.as_corner(context),
        .bottom_left = border_radii.bottom_left.as_corner(context)
    };
    m_has_radius = corner_radii.has_any_radius();
    if (!m_has_radius)
        return;
    m_context.display_list_recorder().sample_under_corners(m_id, corner_radii, border_rect.to_type<int>(), corner_clip);
}

ScopedCornerRadiusClip::~ScopedCornerRadiusClip()
{
    if (!m_has_radius)
        return;
    m_context.display_list_recorder().blit_corner_clipping(m_id);
}

}
