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

void StyleBoxFlat::set_light_color(const Color &p_color) {

	light_color = p_color;
	emit_changed();
}
void StyleBoxFlat::set_dark_color(const Color &p_color) {

	dark_color = p_color;
	emit_changed();
}

Color StyleBoxFlat::get_bg_color() const {

	return bg_color;
}
Color StyleBoxFlat::get_light_color() const {

	return light_color;
}
Color StyleBoxFlat::get_dark_color() const {

	return dark_color;
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

	draw_center = p_draw;
	emit_changed();
}
bool StyleBoxFlat::get_draw_center() const {

	return draw_center;
}
Size2 StyleBoxFlat::get_center_size() const {

	return Size2();
}

void StyleBoxFlat::draw(RID p_canvas_item, const Rect2 &p_rect) const {

	VisualServer *vs = VisualServer::get_singleton();
	Rect2i r = p_rect;
	
//drawing border
	for (int i = 0; i < border_size; i++) {

		Color color_upleft = light_color;
		Color color_downright = dark_color;

		if (blend) {

			color_upleft.r = (border_size - i) * color_upleft.r / border_size + i * bg_color.r / border_size;
			color_upleft.g = (border_size - i) * color_upleft.g / border_size + i * bg_color.g / border_size;
			color_upleft.b = (border_size - i) * color_upleft.b / border_size + i * bg_color.b / border_size;

			color_downright.r = (border_size - i) * color_downright.r / border_size + i * bg_color.r / border_size;
			color_downright.g = (border_size - i) * color_downright.g / border_size + i * bg_color.g / border_size;
			color_downright.b = (border_size - i) * color_downright.b / border_size + i * bg_color.b / border_size;
		}

		vs->canvas_item_add_rect(p_canvas_item, Rect2(Point2i(r.pos.x, r.pos.y + r.size.y - 1), Size2(r.size.x, 1)), color_downright);
		vs->canvas_item_add_rect(p_canvas_item, Rect2(Point2i(r.pos.x + r.size.x - 1, r.pos.y), Size2(1, r.size.y)), color_downright);

		vs->canvas_item_add_rect(p_canvas_item, Rect2(r.pos, Size2(r.size.x, 1)), color_upleft);
		vs->canvas_item_add_rect(p_canvas_item, Rect2(r.pos, Size2(1, r.size.y)), color_upleft);

		r.pos.x++;
		r.pos.y++;
		r.size.x -= 2;
		r.size.y -= 2;
	}

	if (draw_center)
		vs->canvas_item_add_rect(p_canvas_item, Rect2(r.pos, r.size), bg_color);

	Rect2i r_add = p_rect;
	vs->canvas_item_add_rect(p_canvas_item, Rect2(Point2i(r_add.pos.x - additional_border_size[MARGIN_LEFT], r_add.pos.y - additional_border_size[MARGIN_TOP]), Size2(r_add.size.width + additional_border_size[MARGIN_LEFT] + additional_border_size[MARGIN_RIGHT], additional_border_size[MARGIN_TOP])), light_color);
	vs->canvas_item_add_rect(p_canvas_item, Rect2(Point2i(r_add.pos.x - additional_border_size[MARGIN_LEFT], r_add.pos.y), Size2(additional_border_size[MARGIN_LEFT], r_add.size.height)), light_color);
	vs->canvas_item_add_rect(p_canvas_item, Rect2(Point2i(r_add.pos.x + r_add.size.width, r_add.pos.y), Size2(additional_border_size[MARGIN_RIGHT], r_add.size.height)), dark_color);
	vs->canvas_item_add_rect(p_canvas_item, Rect2(Point2i(r_add.pos.x - additional_border_size[MARGIN_LEFT], r_add.pos.y + r_add.size.height), Size2(r_add.size.width + additional_border_size[MARGIN_LEFT] + additional_border_size[MARGIN_RIGHT], additional_border_size[MARGIN_BOTTOM])), dark_color);
}

float StyleBoxFlat::get_style_margin(Margin p_margin) const {

	return border_size;
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

	ADD_PROPERTY(PropertyInfo(Variant::COLOR, "bg_color"), "set_bg_color", "get_bg_color");
	ADD_PROPERTY(PropertyInfo(Variant::COLOR, "light_color"), "set_light_color", "get_light_color");
	ADD_PROPERTY(PropertyInfo(Variant::COLOR, "dark_color"), "set_dark_color", "get_dark_color");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "border_size", PROPERTY_HINT_RANGE, "0,4096"), "set_border_size", "get_border_size");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "border_blend"), "set_border_blend", "get_border_blend");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "draw_bg"), "set_draw_center", "get_draw_center");
}

StyleBoxFlat::StyleBoxFlat() {

	bg_color = Color(0.6, 0.6, 0.6);
	light_color = Color(0.8, 0.8, 0.8);
	dark_color = Color(0.8, 0.8, 0.8);
	draw_center = true;
	blend = true;
	border_size = 0;
	additional_border_size[0] = 0;
	additional_border_size[1] = 0;
	additional_border_size[2] = 0;
	additional_border_size[3] = 0;
}
StyleBoxFlat::~StyleBoxFlat() {
}


///////////

//REF
/*
	func draw_rounded_rect_without_border(rect, corner_radius, col_light, col_dark, border_for_reference = 0):
	var i = 0
	for cr in corner_radius:
	if cr * 2 > rect.size.height:
	corner_radius[i] = rect.size.height / 2
	if cr * 2 > rect.size.width:
	corner_radius[i] = rect.size.width / 2
	i += 1
 #	rect = rect.grow( -corner_radius )
	var tl = rect.pos + Vector2(corner_radius[0], corner_radius[0])
	var tr = Vector2(rect.pos.x + rect.size.x - corner_radius[1], rect.pos.y + corner_radius[1])
	var br = rect.pos + rect.size - Vector2(corner_radius[2], corner_radius[2])
	var bl = Vector2(rect.pos.x + corner_radius[3], rect.pos.y + rect.size.y - corner_radius[3])
	var corners = [tl,tr,br,bl]
	i = 0
	for corner_pos in corners:
	var col = col_light
	if i > 1:
	col = col_dark
	draw_circle(corner_pos, corner_radius[i], col)
	i += 1
 #	var rect_vertical = Rect2(rect.pos.x, rect.pos.y - corner_radius, rect.size.width, rect.size.height + 2 * corner_radius)
 #	var rect_horizont = Rect2(rect.pos.x - corner_radius, rect.pos.y, rect.size.width + 2 * corner_radius, rect.size.height)
 #	if corner_radius[2] < border_for_reference:
 #		corner_radius[2] = border_for_reference
	var rect_bottom_height = border_for_reference
	if border_for_reference == 0:
	rect_bottom_height = rect.size.height/2
	var rect_top =		Rect2(rect.pos.x + corner_radius[0], 	rect.pos.y, 											rect.size.width - corner_radius[0] - corner_radius[1], rect.size.height/2)
	var rect_bottom =	Rect2(rect.pos.x + corner_radius[3], 	rect.pos.y + rect.size.height - rect_bottom_height, 	rect.size.width - corner_radius[3] - corner_radius[2], rect_bottom_height)
	var rect_left = 	Rect2(rect.pos.x, 						rect.pos.y + corner_radius[0], rect.size.width/2, 		rect.size.height - corner_radius[0] - corner_radius[3])
	var rect_right = 	Rect2(rect.pos.x + rect.size.width/2, 	rect.pos.y + corner_radius[1], rect.size.width/2, 		rect.size.height - corner_radius[1] - corner_radius[2])
	draw_rect(rect_top, col_light)
	draw_rect(rect_left, col_light)
	draw_rect(rect_right, col_light)
	draw_rect(rect_bottom, col_dark)
	
	func draw_rounded_rect(rect, corner_radius, color, border_width, border_color_light, border_color_dark, blend_border):
	if blend_border:
	for i in range(0, border_width):
	var factor = float(i) / float(border_width)
	interp(border_color_light, color, i, border_width)
	var interp_color_light = border_color_light.linear_interpolate(color, factor)
	var interp_color_dark = border_color_dark.linear_interpolate(color, factor)
	var inner_rect = rect.grow(-i)
	if inner_rect.size.width > 0 and inner_rect.size.height > 0:
	draw_rounded_rect_without_border(inner_rect, corner_radius, interp_color_light, interp_color_dark, border_width)
	else:
	draw_rounded_rect_without_border(rect, corner_radius, border_color_light, border_color_dark, border_width)
	
	var inner_rect = rect.grow(-border_width)
	if inner_rect.size.width > 0 and inner_rect.size.height > 0:
	draw_rounded_rect_without_border(inner_rect, corner_radius, color, color)
	
	
 */
//REF
inline void draw_rounded_rect_without_border(VisualServer *vs, RID p_canvas_item, Rect2 rect, int corner_radius[4], Color col_light, Color col_dark, int border_for_reference = 0){
	for(int i = 0; i < 4; i++){
		if(corner_radius[i] * 2 > rect.size.height)
			corner_radius[i] = rect.size.height / 2;
		if(corner_radius[i] * 2 > rect.size.width)
			corner_radius[i] = rect.size.width / 2;
	}
	
	Vector2 tl = rect.pos + Vector2(corner_radius[0], corner_radius[0]);
	Vector2 tr = Vector2(rect.pos.x + rect.size.x - corner_radius[1], rect.pos.y + corner_radius[1]);
	Vector2 br = rect.pos + rect.size - Vector2(corner_radius[2], corner_radius[2]);
	Vector2 bl = Vector2(rect.pos.x + corner_radius[3], rect.pos.y + rect.size.y - corner_radius[3]);
	
	PoolVector2Array corners = PoolVector2Array();
	corners.append(tl);
	corners.append(tr);
	corners.append(br);
	corners.append(bl);
	
	for(int i = 0; i < 4; i++){
		
		Color col = col_light;
		if (i > 1)
			col = col_dark;
		vs->canvas_item_add_circle(p_canvas_item, corners[i], corner_radius[i], col);
	}
	
	int rect_bottom_height = border_for_reference;
	if (border_for_reference == 0)
		rect_bottom_height = rect.size.height/2;
	Rect2 rect_top = Rect2(rect.pos.x + corner_radius[0], rect.pos.y, rect.size.width - corner_radius[0] - corner_radius[1], rect.size.height/2);
	Rect2 rect_bottom = Rect2(rect.pos.x + corner_radius[3], rect.pos.y + rect.size.height - rect_bottom_height, rect.size.width - corner_radius[3] - corner_radius[2], rect_bottom_height);
	Rect2 rect_left = Rect2(rect.pos.x, rect.pos.y + corner_radius[0], rect.size.width/2, rect.size.height - corner_radius[0] - corner_radius[3]);
	Rect2 rect_right = Rect2(rect.pos.x + rect.size.width/2, rect.pos.y + corner_radius[1], rect.size.width/2, rect.size.height - corner_radius[1] - corner_radius[2]);
	
	vs->canvas_item_add_rect(p_canvas_item, rect_top, col_light);
	vs->canvas_item_add_rect(p_canvas_item, rect_left, col_light);
	vs->canvas_item_add_rect(p_canvas_item, rect_right, col_light);
	vs->canvas_item_add_rect(p_canvas_item, rect_bottom, col_dark);
}
inline void draw_rounded_rect(VisualServer *vs, RID p_canvas_item, Rect2i rect, int corner_radius[4], const Color color, const int border_width, const Color border_color_light, const Color border_color_dark, const bool blend_border){
	if (blend_border){
		for(int i = 0; i < border_width; i++){
			float factor = float(i) / float(border_width);
			Color interp_color_light = border_color_light.linear_interpolate(color, factor);
			Color interp_color_dark = border_color_dark.linear_interpolate(color, factor);
			Rect2i inner_rect = rect.grow(-i);
			if (inner_rect.size.width > 0 && inner_rect.size.height > 0){
				draw_rounded_rect_without_border(vs, p_canvas_item, inner_rect, corner_radius, interp_color_light, interp_color_dark, border_width);
			}else{
				draw_rounded_rect_without_border(vs, p_canvas_item, rect, corner_radius, border_color_light, border_color_dark, border_width);
				break;
			}
		}
	}
	Rect2 inner_rect = rect.grow(-border_width);
	if (inner_rect.size.width > 0 && inner_rect.size.height > 0)
		draw_rounded_rect_without_border(vs, p_canvas_item, inner_rect, corner_radius, color, color);
	
}

void StyleBoxRound::draw(RID p_canvas_item, const Rect2 &p_rect) const{
	
	VisualServer *vs = VisualServer::get_singleton();
	Rect2i r = p_rect;
	
	draw_rounded_rect(*vs, p_canvas_item, r, get_bg_color(), get_border_size(), get_light_color(), get_dark_color(), get_border_blend());
}
int StyleBoxRound::get_corner_radius_TL() const{
	
	return corner_radius[0];
}
int StyleBoxRound::get_corner_radius_TR() const{
	
	return corner_radius[1];
}
int StyleBoxRound::get_corner_radius_BR() const{
	
	return corner_radius[2];
}
int StyleBoxRound::get_corner_radius_BL() const{
	
	return corner_radius[3];
}
int StyleBoxRound::get_smallest_corner_radius() const{
	int smallest = corner_radius[0];
	for(int i = 1; i < 4; i++){
		if(smallest > corner_radius[i]){
			smallest = corner_radius[i];
		}
	}
	return smallest;
}
void StyleBoxRound::set_corner_radius(int radius){
	
	for(int i = 0; i < 4; i++){
		corner_radius[i] = radius;
	}
}
void StyleBoxRound::set_corner_radius_TL(int radius){
	
	corner_radius[0] = radius;
}
void StyleBoxRound::set_corner_radius_TR(int radius){
	
	corner_radius[1] = radius;
}
void StyleBoxRound::set_corner_radius_BR(int radius){
	
	corner_radius[2] = radius;
}
void StyleBoxRound::set_corner_radius_BL(int radius){
	
	corner_radius[3] = radius;
}
StyleBoxRound::StyleBoxRound() {

	StyleBoxFlat::StyleBoxFlat();
	corner_radius[0] = 0;
	corner_radius[1] = 0;
	corner_radius[2] = 0;
	corner_radius[3] = 0;
}
StyleBoxRound::~StyleBoxRound() {
}
