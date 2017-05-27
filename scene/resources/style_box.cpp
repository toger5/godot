/*************************************************************************/
/*  style_box.cpp                                                        */
/*************************************************************************/
/*                       This file is part of:                           */
/*                           GODOT ENGINE                                */
/*                    http://www.godotengine.org                         */
/*************************************************************************/
/* Copyright (c) 2007-2017 Juan Linietsky, Ariel Manzur.                 */
/* Copyright (c) 2014-2017 Godot Engine contributors (cf. AUTHORS.md)    */
/*                                                                       */
/* Permission is hereby granted, free of charge, to any person obtaining */
/* a copy of this software and associated documentation files (the       */
/* "Software"), to deal in the Software without restriction, including   */
/* without limitation the rights to use, copy, modify, merge, publish,   */
/* distribute, sublicense, and/or sell copies of the Software, and to    */
/* permit persons to whom the Software is furnished to do so, subject to */
/* the following conditions:                                             */
/*                                                                       */
/* The above copyright notice and this permission notice shall be        */
/* included in all copies or substantial portions of the Software.       */
/*                                                                       */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,       */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF    */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.*/
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY  */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,  */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE     */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                */
/*************************************************************************/
#include "style_box.h"

bool StyleBox::test_mask(const Point2 &p_point, const Rect2 &p_rect) const {

	return true;
}

void StyleBox::set_default_margin(Margin p_margin, float p_value) {

	margin[p_margin] = p_value;
	emit_changed();
}
float StyleBox::get_default_margin(Margin p_margin) const {

	return margin[p_margin];
}

float StyleBox::get_margin(Margin p_margin) const {

	if (margin[p_margin] < 0)
		return get_style_margin(p_margin);
	else
		return margin[p_margin];
}

Size2 StyleBox::get_minimum_size() const {

	return Size2(get_margin(MARGIN_LEFT) + get_margin(MARGIN_RIGHT), get_margin(MARGIN_TOP) + get_margin(MARGIN_BOTTOM));
}

Point2 StyleBox::get_offset() const {

	return Point2(get_margin(MARGIN_LEFT), get_margin(MARGIN_TOP));
}

Size2 StyleBox::get_center_size() const {

	return Size2();
}

void StyleBox::_bind_methods() {

	ClassDB::bind_method(D_METHOD("test_mask", "point", "rect"), &StyleBox::test_mask);

	ClassDB::bind_method(D_METHOD("set_default_margin", "margin", "offset"), &StyleBox::set_default_margin);
	ClassDB::bind_method(D_METHOD("get_default_margin", "margin"), &StyleBox::get_default_margin);

	//ClassDB::bind_method(D_METHOD("set_default_margin"),&StyleBox::set_default_margin);
	//ClassDB::bind_method(D_METHOD("get_default_margin"),&StyleBox::get_default_margin);

	ClassDB::bind_method(D_METHOD("get_margin", "margin"), &StyleBox::get_margin);
	ClassDB::bind_method(D_METHOD("get_minimum_size"), &StyleBox::get_minimum_size);
	ClassDB::bind_method(D_METHOD("get_center_size"), &StyleBox::get_center_size);
	ClassDB::bind_method(D_METHOD("get_offset"), &StyleBox::get_offset);

	ClassDB::bind_method(D_METHOD("draw", "canvas_item", "rect"), &StyleBox::draw);

	ADD_GROUP("Content Margin", "content_margin_");
	ADD_PROPERTYI(PropertyInfo(Variant::REAL, "content_margin_left", PROPERTY_HINT_RANGE, "-1,2048,1"), "set_default_margin", "get_default_margin", MARGIN_LEFT);
	ADD_PROPERTYI(PropertyInfo(Variant::REAL, "content_margin_right", PROPERTY_HINT_RANGE, "-1,2048,1"), "set_default_margin", "get_default_margin", MARGIN_RIGHT);
	ADD_PROPERTYI(PropertyInfo(Variant::REAL, "content_margin_top", PROPERTY_HINT_RANGE, "-1,2048,1"), "set_default_margin", "get_default_margin", MARGIN_TOP);
	ADD_PROPERTYI(PropertyInfo(Variant::REAL, "content_margin_bottom", PROPERTY_HINT_RANGE, "-1,2048,1"), "set_default_margin", "get_default_margin", MARGIN_BOTTOM);
}

StyleBox::StyleBox() {

	for (int i = 0; i < 4; i++) {

		margin[i] = -1;
	}
}

void StyleBoxTexture::set_texture(RES p_texture) {

	if (texture == p_texture)
		return;
	texture = p_texture;
	region_rect = Rect2(Point2(), texture->get_size());
	emit_signal("texture_changed");
	emit_changed();
}

RES StyleBoxTexture::get_texture() const {

	return texture;
}

void StyleBoxTexture::set_margin_size(Margin p_margin, float p_size) {

	margin[p_margin] = p_size;
	emit_changed();
}
float StyleBoxTexture::get_margin_size(Margin p_margin) const {

	return margin[p_margin];
}

float StyleBoxTexture::get_style_margin(Margin p_margin) const {

	return margin[p_margin];
}

void StyleBoxTexture::draw(RID p_canvas_item, const Rect2 &p_rect) const {
	if (texture.is_null())
		return;

	Rect2 rect = p_rect;
	Rect2 src_rect = region_rect;

	texture->get_rect_region(rect, src_rect, rect, src_rect);

	rect.pos.x -= expand_margin[MARGIN_LEFT];
	rect.pos.y -= expand_margin[MARGIN_TOP];
	rect.size.x += expand_margin[MARGIN_LEFT] + expand_margin[MARGIN_RIGHT];
	rect.size.y += expand_margin[MARGIN_TOP] + expand_margin[MARGIN_BOTTOM];

	VisualServer::get_singleton()->canvas_item_add_nine_patch(p_canvas_item, rect, src_rect, texture->get_rid(), Vector2(margin[MARGIN_LEFT], margin[MARGIN_TOP]), Vector2(margin[MARGIN_RIGHT], margin[MARGIN_BOTTOM]), VS::NINE_PATCH_STRETCH, VS::NINE_PATCH_STRETCH, draw_center, modulate);
}

void StyleBoxTexture::set_draw_center(bool p_draw) {

	draw_center = p_draw;
	emit_changed();
}

bool StyleBoxTexture::get_draw_center() const {

	return draw_center;
}

Size2 StyleBoxTexture::get_center_size() const {

	if (texture.is_null())
		return Size2();

	return texture->get_size() - get_minimum_size();
}

void StyleBoxTexture::set_expand_margin_size(Margin p_expand_margin, float p_size) {

	ERR_FAIL_INDEX(p_expand_margin, 4);
	expand_margin[p_expand_margin] = p_size;
	emit_changed();
}

float StyleBoxTexture::get_expand_margin_size(Margin p_expand_margin) const {

	ERR_FAIL_INDEX_V(p_expand_margin, 4, 0);
	return expand_margin[p_expand_margin];
}

void StyleBoxTexture::set_region_rect(const Rect2 &p_region_rect) {

	if (region_rect == p_region_rect)
		return;

	region_rect = p_region_rect;
	emit_changed();
}

Rect2 StyleBoxTexture::get_region_rect() const {

	return region_rect;
}

void StyleBoxTexture::set_modulate(const Color &p_modulate) {
	if (modulate == p_modulate)
		return;
	modulate = p_modulate;
	emit_changed();
}

Color StyleBoxTexture::get_modulate() const {

	return modulate;
}

void StyleBoxTexture::_bind_methods() {

	ClassDB::bind_method(D_METHOD("set_texture", "texture:Texture"), &StyleBoxTexture::set_texture);
	ClassDB::bind_method(D_METHOD("get_texture:Texture"), &StyleBoxTexture::get_texture);

	ClassDB::bind_method(D_METHOD("set_margin_size", "margin", "size"), &StyleBoxTexture::set_margin_size);
	ClassDB::bind_method(D_METHOD("get_margin_size", "margin"), &StyleBoxTexture::get_margin_size);

	ClassDB::bind_method(D_METHOD("set_expand_margin_size", "margin", "size"), &StyleBoxTexture::set_expand_margin_size);
	ClassDB::bind_method(D_METHOD("get_expand_margin_size", "margin"), &StyleBoxTexture::get_expand_margin_size);

	ClassDB::bind_method(D_METHOD("set_region_rect", "region"), &StyleBoxTexture::set_region_rect);
	ClassDB::bind_method(D_METHOD("get_region_rect"), &StyleBoxTexture::get_region_rect);

	ClassDB::bind_method(D_METHOD("set_draw_center", "enable"), &StyleBoxTexture::set_draw_center);
	ClassDB::bind_method(D_METHOD("get_draw_center"), &StyleBoxTexture::get_draw_center);

	ClassDB::bind_method(D_METHOD("set_modulate", "color"), &StyleBoxTexture::set_modulate);
	ClassDB::bind_method(D_METHOD("get_modulate"), &StyleBoxTexture::get_modulate);

	ADD_SIGNAL(MethodInfo("texture_changed"));

	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "texture", PROPERTY_HINT_RESOURCE_TYPE, "Texture"), "set_texture", "get_texture");
	ADD_PROPERTYNZ(PropertyInfo(Variant::RECT2, "region_rect"), "set_region_rect", "get_region_rect");
	ADD_GROUP("Margin", "margin_");
	ADD_PROPERTYI(PropertyInfo(Variant::REAL, "margin_left", PROPERTY_HINT_RANGE, "0,2048,1"), "set_margin_size", "get_margin_size", MARGIN_LEFT);
	ADD_PROPERTYI(PropertyInfo(Variant::REAL, "margin_right", PROPERTY_HINT_RANGE, "0,2048,1"), "set_margin_size", "get_margin_size", MARGIN_RIGHT);
	ADD_PROPERTYI(PropertyInfo(Variant::REAL, "margin_top", PROPERTY_HINT_RANGE, "0,2048,1"), "set_margin_size", "get_margin_size", MARGIN_TOP);
	ADD_PROPERTYI(PropertyInfo(Variant::REAL, "margin_bottom", PROPERTY_HINT_RANGE, "0,2048,1"), "set_margin_size", "get_margin_size", MARGIN_BOTTOM);
	ADD_GROUP("Expand Margin", "expand_margin_");
	ADD_PROPERTYI(PropertyInfo(Variant::REAL, "expand_margin_left", PROPERTY_HINT_RANGE, "0,2048,1"), "set_expand_margin_size", "get_expand_margin_size", MARGIN_LEFT);
	ADD_PROPERTYI(PropertyInfo(Variant::REAL, "expand_margin_right", PROPERTY_HINT_RANGE, "0,2048,1"), "set_expand_margin_size", "get_expand_margin_size", MARGIN_RIGHT);
	ADD_PROPERTYI(PropertyInfo(Variant::REAL, "expand_margin_top", PROPERTY_HINT_RANGE, "0,2048,1"), "set_expand_margin_size", "get_expand_margin_size", MARGIN_TOP);
	ADD_PROPERTYI(PropertyInfo(Variant::REAL, "expand_margin_bottom", PROPERTY_HINT_RANGE, "0,2048,1"), "set_expand_margin_size", "get_expand_margin_size", MARGIN_BOTTOM);
	ADD_GROUP("Modulate", "modulate_");
	ADD_PROPERTY(PropertyInfo(Variant::COLOR, "modulate_color"), "set_modulate", "get_modulate");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "draw_center"), "set_draw_center", "get_draw_center");
}

StyleBoxTexture::StyleBoxTexture() {

	for (int i = 0; i < 4; i++) {
		margin[i] = 0;
		expand_margin[i] = 0;
	}
	draw_center = true;
	modulate = Color(1, 1, 1, 1);
}
StyleBoxTexture::~StyleBoxTexture() {
}

////////////////

void StyleBoxFlat::set_bg_color(const Color &p_color) {

	bg_color = p_color;
	emit_changed();
}

void StyleBoxFlat::set_border_color(const Color &p_color, const Margin &p_border) {

	border_color.write()[p_border] = p_color;
	emit_changed();
}
void StyleBoxFlat::set_border_color(const Color &p_color) {
	for (int i; i < 4; i++) {

		border_color.write()[i] = p_color;
	}
	emit_changed();
}
void StyleBoxFlat::set_light_color(const Color &p_color) {

	set_border_color(p_color, MARGIN_LEFT);
	set_border_color(p_color, MARGIN_TOP);
	set_border_color(p_color, MARGIN_RIGHT);
	emit_changed();
}
void StyleBoxFlat::set_dark_color(const Color &p_color) {
	set_border_color(p_color, MARGIN_BOTTOM);
	emit_changed();
}
Color StyleBoxFlat::get_bg_color() const {

	return bg_color;
}
Color StyleBoxFlat::get_border_color(const Margin &p_border) const {

	return border_color[p_border];
}
Color StyleBoxFlat::get_light_color() const {

	return get_border_color(MARGIN_TOP);
}
Color StyleBoxFlat::get_dark_color() const {

	return get_border_color(MARGIN_BOTTOM);
}

void StyleBoxFlat::set_border_size(int p_size) {
	border_size = p_size;
	emit_changed();
}
int StyleBoxFlat::get_border_size() const {

	return border_size;
}

void StyleBoxFlat::_set_additional_border_size(Margin p_margin, int p_size) {
	additional_border_size[p_margin] = p_size;
	emit_changed();
}

int StyleBoxFlat::_get_additional_border_size(Margin p_margin) const {
	return additional_border_size[p_margin];
}

void StyleBoxFlat::set_border_blend(bool p_blend) {

	blend = p_blend;
	emit_changed();
}

bool StyleBoxFlat::get_border_blend() const {

	return blend;
}
void StyleBoxFlat::set_draw_center(bool p_draw) {

	bg_color = Color(0, 0, 0, 0);
	emit_changed();
}
bool StyleBoxFlat::get_draw_center() const {

	return bg_color.a != 0;
}
Size2 StyleBoxFlat::get_center_size() const {

	return Size2();
}
int StyleBoxFlat::get_corner_radius_TL() const {

	return corner_radius[0];
}
int StyleBoxFlat::get_corner_radius_TR() const {

	return corner_radius[1];
}
int StyleBoxFlat::get_corner_radius_BR() const {

	return corner_radius[2];
}
int StyleBoxFlat::get_corner_radius_BL() const {

	return corner_radius[3];
}

PoolIntArray StyleBoxFlat::get_corner_radius() const {
	PoolIntArray arr;
	arr.append(corner_radius[0]);
	arr.append(corner_radius[1]);
	arr.append(corner_radius[2]);
	arr.append(corner_radius[3]);
	return arr;
}

int StyleBoxFlat::get_smallest_corner_radius() const {
	int smallest = corner_radius[0];
	for (int i = 1; i < 4; i++) {
		if (smallest > corner_radius[i]) {
			smallest = corner_radius[i];
		}
	}
	return smallest;
}
void StyleBoxFlat::set_all_corner_radius(int radius) {

	for (int i = 0; i < 4; i++) {
		corner_radius[i] = radius;
	}
	emit_changed();
}
void StyleBoxFlat::set_corner_radius(const int radius_top_left, const int radius_top_right, const int radius_botton_right, const int radius_bottom_left) {
	corner_radius[0] = radius_top_left;
	corner_radius[1] = radius_top_right;
	corner_radius[2] = radius_botton_right;
	corner_radius[3] = radius_bottom_left;
	emit_changed();
}
void StyleBoxFlat::set_corner_radius_TL(int radius) {

	corner_radius[0] = radius;
	emit_changed();
}
void StyleBoxFlat::set_corner_radius_TR(int radius) {

	corner_radius[1] = radius;
	emit_changed();
}
void StyleBoxFlat::set_corner_radius_BR(int radius) {

	corner_radius[2] = radius;
	emit_changed();
}
void StyleBoxFlat::set_corner_radius_BL(int radius) {

	corner_radius[3] = radius;
	emit_changed();
}
float StyleBoxFlat::get_style_margin(Margin p_margin) const {
	int margin_size = border_size;
	switch (p_margin) {
		case MARGIN_TOP:
			if (get_corner_radius_TL() / 2 > margin_size)
				margin_size = get_corner_radius_TL() / 2;
			if (get_corner_radius_TR() / 2 > margin_size)
				margin_size = get_corner_radius_TR() / 2;
		case MARGIN_BOTTOM:
			if (get_corner_radius_BL() / 2 > margin_size)
				margin_size = get_corner_radius_BL() / 2;
			if (get_corner_radius_BR() / 2 > margin_size)
				margin_size = get_corner_radius_BR() / 2;
		case MARGIN_LEFT:
			if (get_corner_radius_TL() / 2 > margin_size)
				margin_size = get_corner_radius_TL() / 2;
			if (get_corner_radius_BL() / 2 > margin_size)
				margin_size = get_corner_radius_BL() / 2;
		case MARGIN_RIGHT:
			if (get_corner_radius_TR() / 2 > margin_size)
				margin_size = get_corner_radius_TR() / 2;
			if (get_corner_radius_BR() / 2 > margin_size)
				margin_size = get_corner_radius_BR() / 2;
	}
	return margin_size;
}
inline void draw_arc(VisualServer *vs, RID p_canvas_item, Point2 pos, int corner_index, float corner_radius, Color col /*, float border*/) {
	int vert_count = 16;
	if (corner_radius > 10) {
		vert_count = 32;
	}
	vert_count = vert_count + (4 - vert_count % 4);
	float offset[4] = { 0.5, 0.75, 0, 0.25 };
	float factor = offset[corner_index];
	float rad = corner_radius;
	Point2 from = Point2(cosf(factor * 2 * Math_PI) * rad, sinf(factor * 2 * Math_PI) * rad) + pos;
	for (int i = 0; i <= vert_count / 4; i++) {
		factor = offset[corner_index] + ((float)i / (float)vert_count);
		Point2 to = Point2(cosf(factor * 2 * Math_PI) * rad, sinf(factor * 2 * Math_PI) * rad) + pos;
		vs->canvas_item_add_line(p_canvas_item, from, to, col, 1, false);
		from = to;
	}
}
inline void draw_rounded_rect(VisualServer *vs, RID p_canvas_item, Rect2 rect, PoolIntArray corner_radius, Color col_top, Color col_bottom, Color col_left, Color col_right, int border, bool filled) {

	PoolIntArray::Write corner_radius_w = corner_radius.write();
	for (int i = 0; i < 4; i++) {
		if (corner_radius[i] * 2 > rect.size.height)
			corner_radius_w[i] = rect.size.height / 2;
		if (corner_radius[i] * 2 > rect.size.width)
			corner_radius_w[i] = rect.size.width / 2;
	}

	Point2 tl = rect.pos + Vector2(corner_radius[0], corner_radius[0]);
	Point2 tr = Point2(rect.pos.x + rect.size.x - corner_radius[1], rect.pos.y + corner_radius[1]);
	Point2 br = rect.pos + rect.size - Vector2(corner_radius[2], corner_radius[2]);
	Point2 bl = Point2(rect.pos.x + corner_radius[3], rect.pos.y + rect.size.y - corner_radius[3]);

	PoolVector2Array corners; // = PoolVector2Array();
	corners.append(tl);
	corners.append(tr);
	corners.append(br);
	corners.append(bl);

	//draw corners
	for (int i = 0; i < 4; i++) {

		float rad = (float)corner_radius[i];

		if (rad == 0) {
			continue;
		}
		Color col = col_top;
		if (i > 1)
			col = col_bottom;
		if (filled) {
			vs->canvas_item_add_circle(p_canvas_item, corners[i], rad, col);
		} else {
			for (float j = 0; j < border; j += 1) {
				if (0 < j && j < border) {
					draw_arc(vs, p_canvas_item, corners[i], i, rad - j + 0.5, col);
				}
				draw_arc(vs, p_canvas_item, corners[i], i, rad - j, col);
			}
		}
	}

	//setup infill values
	int border_left_right = border;
	int border_top_bottom = border;
	if (filled) {
		border_top_bottom = rect.size.height / 2;
		border_left_right = rect.size.width / 2;
	}
	Rect2 rect_top = Rect2(rect.pos.x + corner_radius[0], rect.pos.y, rect.size.width - corner_radius[0] - corner_radius[1], border_top_bottom);
	Rect2 rect_bottom = Rect2(rect.pos.x + corner_radius[3], rect.pos.y + rect.size.height - border_top_bottom, rect.size.width - corner_radius[3] - corner_radius[2], border_top_bottom);
	Rect2 rect_left = Rect2(rect.pos.x, rect.pos.y + corner_radius[0], border_left_right, rect.size.height - corner_radius[0] - corner_radius[3]);
	Rect2 rect_right = Rect2(rect.pos.x + rect.size.width - border_left_right, rect.pos.y + corner_radius[1], border_left_right, rect.size.height - corner_radius[1] - corner_radius[2]);

	vs->canvas_item_add_rect(p_canvas_item, rect_top, col_top);
	vs->canvas_item_add_rect(p_canvas_item, rect_left, col_left);
	vs->canvas_item_add_rect(p_canvas_item, rect_right, col_right);
	//has to be drawn last
	vs->canvas_item_add_rect(p_canvas_item, rect_bottom, col_bottom);
}
inline PoolIntArray get_offset_corner_radius(int offset, PoolIntArray corner_radius) {
	PoolIntArray inner_corner_radius = PoolIntArray();
	for (int i = 0; i < 4; i++) {
		int rad = corner_radius[i] - offset;
		if (rad < 0)
			rad = 0;
		inner_corner_radius.append(rad);
	}
	return inner_corner_radius;
}
inline void draw_rounded_rect_bordered(VisualServer *vs, RID p_canvas_item, Rect2i rect, PoolIntArray corner_radius, Color color, int border_width, PoolColorArray b_col, bool blend_border) {

	if (blend_border && border_width > 0) {
		int step = 1;
		if (border_width > 15) {
			step = border_width / 15;
		}
		for (int i = 0; i < border_width; i += step) {
			float factor = 1.0 - (float(i) / float(border_width));
			PoolColorArray interp_color_array;
			for (int j = 0; j < 4; j++) {
				interp_color_array.append(color.linear_interpolate(b_col.read()[j], factor));
			}
			Rect2i inner_rectt = rect.grow(-i);
			if (inner_rectt.size.width > 0 && inner_rectt.size.height > 0) {
				draw_rounded_rect(vs, p_canvas_item, inner_rectt, get_offset_corner_radius(i, corner_radius), interp_color_array[MARGIN_TOP], interp_color_array[MARGIN_BOTTOM], interp_color_array[MARGIN_LEFT], interp_color_array[MARGIN_RIGHT], border_width, false);
			} else {
				break;
			}
		}
	} else {
		draw_rounded_rect(vs, p_canvas_item, rect, get_offset_corner_radius(0, corner_radius), b_col[MARGIN_TOP], b_col[MARGIN_BOTTOM], b_col[MARGIN_LEFT], b_col[MARGIN_RIGHT], border_width, false);
	}
	Rect2 inner_rect = rect.grow(-border_width);
	if (inner_rect.size.width > 0 && inner_rect.size.height > 0) {
		//		draw_rounded_rect(vs, p_canvas_item, inner_rect, get_offset_corner_radius(border_width, corner_radius), color, color, color, color, border_width, true);
		draw_rounded_rect(vs, p_canvas_item, inner_rect, get_offset_corner_radius(border_width, corner_radius), color, color, color, color, border_width, true);
	};
}
void StyleBoxFlat::draw(RID p_canvas_item, const Rect2 &p_rect) const {

	VisualServer *vs = VisualServer::get_singleton();
	Rect2i r = p_rect;
	PoolIntArray cr;
	cr.append(corner_radius[0]);
	cr.append(corner_radius[1]);
	cr.append(corner_radius[2]);
	cr.append(corner_radius[3]);

	int adapted_border = border_size;
	if (adapted_border > r.size.width) {
		adapted_border = r.size.width;
	}
	if (adapted_border > r.size.height) {
		adapted_border = r.size.height;
	}
	draw_rounded_rect_bordered(vs, p_canvas_item, r, cr, bg_color, adapted_border, border_color, blend);

	//draw additional borders
	Rect2i r_add = p_rect;
	//top border
	vs->canvas_item_add_rect(p_canvas_item, Rect2(Point2i(r_add.pos.x - additional_border_size[MARGIN_LEFT], r_add.pos.y - additional_border_size[MARGIN_TOP]), Size2(r_add.size.width + additional_border_size[MARGIN_LEFT] + additional_border_size[MARGIN_RIGHT], additional_border_size[MARGIN_TOP])), border_color[MARGIN_TOP]);
	//left border
	vs->canvas_item_add_rect(p_canvas_item, Rect2(Point2i(r_add.pos.x - additional_border_size[MARGIN_LEFT], r_add.pos.y), Size2(additional_border_size[MARGIN_LEFT], r_add.size.height)), border_color[MARGIN_LEFT]);
	//right border
	vs->canvas_item_add_rect(p_canvas_item, Rect2(Point2i(r_add.pos.x + r_add.size.width, r_add.pos.y), Size2(additional_border_size[MARGIN_RIGHT], r_add.size.height)), border_color[MARGIN_RIGHT]);
	//bottom border
	vs->canvas_item_add_rect(p_canvas_item, Rect2(Point2i(r_add.pos.x - additional_border_size[MARGIN_LEFT], r_add.pos.y + r_add.size.height), Size2(r_add.size.width + additional_border_size[MARGIN_LEFT] + additional_border_size[MARGIN_RIGHT], additional_border_size[MARGIN_BOTTOM])), border_color[MARGIN_BOTTOM]);
}
void StyleBoxFlat::_bind_methods() {

	ClassDB::bind_method(D_METHOD("set_bg_color", "color"), &StyleBoxFlat::set_bg_color);
	ClassDB::bind_method(D_METHOD("get_bg_color"), &StyleBoxFlat::get_bg_color);
	ClassDB::bind_method(D_METHOD("set_light_color", "color"), &StyleBoxFlat::set_light_color);
	ClassDB::bind_method(D_METHOD("get_light_color"), &StyleBoxFlat::get_light_color);
	ClassDB::bind_method(D_METHOD("set_dark_color", "color"), &StyleBoxFlat::set_dark_color);
	ClassDB::bind_method(D_METHOD("get_dark_color"), &StyleBoxFlat::get_dark_color);
	ClassDB::bind_method(D_METHOD("set_border_size", "size"), &StyleBoxFlat::set_border_size);
	ClassDB::bind_method(D_METHOD("get_border_size"), &StyleBoxFlat::get_border_size);
	ClassDB::bind_method(D_METHOD("set_border_blend", "blend"), &StyleBoxFlat::set_border_blend);
	ClassDB::bind_method(D_METHOD("get_border_blend"), &StyleBoxFlat::get_border_blend);
	ClassDB::bind_method(D_METHOD("set_draw_center", "size"), &StyleBoxFlat::set_draw_center);
	ClassDB::bind_method(D_METHOD("get_draw_center"), &StyleBoxFlat::get_draw_center);

	ClassDB::bind_method(D_METHOD("set_corner_radius_TL", "radius"), &StyleBoxFlat::set_corner_radius_TL);
	ClassDB::bind_method(D_METHOD("set_corner_radius_TR", "radius"), &StyleBoxFlat::set_corner_radius_TR);
	ClassDB::bind_method(D_METHOD("set_corner_radius_BL", "radius"), &StyleBoxFlat::set_corner_radius_BL);
	ClassDB::bind_method(D_METHOD("set_corner_radius_BR", "radius"), &StyleBoxFlat::set_corner_radius_BR);

	ClassDB::bind_method(D_METHOD("get_corner_radius_TL"), &StyleBoxFlat::get_corner_radius_TL);
	ClassDB::bind_method(D_METHOD("get_corner_radius_TR"), &StyleBoxFlat::get_corner_radius_TR);
	ClassDB::bind_method(D_METHOD("get_corner_radius_BL"), &StyleBoxFlat::get_corner_radius_BL);
	ClassDB::bind_method(D_METHOD("get_corner_radius_BR"), &StyleBoxFlat::get_corner_radius_BR);

	ClassDB::bind_method(D_METHOD("set_corner_radius", "radius_top_left", "radius_top_right", "radius_botton_right", "radius_bottom_left"), &StyleBoxFlat::set_corner_radius);
	ClassDB::bind_method(D_METHOD("set_all_corner_radius", "radius"), &StyleBoxFlat::set_all_corner_radius);

	ADD_PROPERTY(PropertyInfo(Variant::INT, "corner_radius_TL"), "set_corner_radius_TL", "get_corner_radius_TL");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "corner_radius_TR"), "set_corner_radius_TR", "get_corner_radius_TR");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "corner_radius_BR"), "set_corner_radius_BR", "get_corner_radius_BR");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "corner_radius_BL"), "set_corner_radius_BL", "get_corner_radius_BL");

	ADD_PROPERTY(PropertyInfo(Variant::COLOR, "bg_color"), "set_bg_color", "get_bg_color");
	ADD_PROPERTY(PropertyInfo(Variant::COLOR, "light_color"), "set_light_color", "get_light_color");
	ADD_PROPERTY(PropertyInfo(Variant::COLOR, "dark_color"), "set_dark_color", "get_dark_color");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "border_size", PROPERTY_HINT_RANGE, "0,4096"), "set_border_size", "get_border_size");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "border_blend"), "set_border_blend", "get_border_blend");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "draw_bg"), "set_draw_center", "get_draw_center");
}

StyleBoxFlat::StyleBoxFlat() {

	bg_color = Color(0.6, 0.6, 0.6);

	border_color.resize(4);
	border_color.write()[0] = Color(0.8, 0.8, 0.8);
	border_color.write()[1] = Color(0.8, 0.8, 0.8);
	border_color.write()[2] = Color(0.8, 0.8, 0.8);
	border_color.write()[3] = Color(0.8, 0.8, 0.8);

	blend = true;
	border_size = 0;
	additional_border_size[0] = 0;
	additional_border_size[1] = 0;
	additional_border_size[2] = 0;
	additional_border_size[3] = 0;

	corner_radius[0] = 0;
	corner_radius[1] = 0;
	corner_radius[2] = 0;
	corner_radius[3] = 0;
}
StyleBoxFlat::~StyleBoxFlat() {
}
