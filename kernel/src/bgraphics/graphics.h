#pragma once

#include "limine.h"
#include <stdint.h>
#include <stddef.h>

volatile uint32_t* framebuffer;
extern struct limine_framebuffer_request framebuffer_request;
volatile struct limine_framebuffer* this_framebuffer;

typedef struct Point
{
    unsigned int x;
    unsigned int y;
} Point;

typedef struct Color
{
    unsigned char r;
    unsigned char g;
    unsigned char b;
} Color;

void initialize_graphics();
void put_pixel(unsigned int x, unsigned int y, unsigned char r, unsigned char g, unsigned char b);
void put_rect(
    unsigned int x,
    unsigned int y,
    unsigned int w,
    unsigned int h,
    unsigned char r,
    unsigned char g,
    unsigned char b
);
void put_line(
    unsigned int x1,
    unsigned int y1,
    unsigned int x2,
    unsigned int y2,
    unsigned char r,
    unsigned char g,
    unsigned char b
);

void put_circle(unsigned int centerX, unsigned int centerY, unsigned int radius, unsigned char r, unsigned char g, unsigned char b);
void put_filled_circle(unsigned int centerX, unsigned int centerY, unsigned int radius, unsigned char r, unsigned char g, unsigned char b);
void put_bitmap_image_data(char* bitmap_data, unsigned int x, unsigned int y, unsigned int width, unsigned int height);
void put_bitmap_image_data_gaussian_blur(char* bitmap_data, unsigned int x, unsigned int y, unsigned int width, unsigned int height);
void put_bitmap_image_data_fit_to(char* bitmap_data, unsigned int x, unsigned int y, unsigned int width, unsigned int height, unsigned int stretched_width, unsigned int stretched_height);