#include "graphics.h"
#include "math.h"

struct limine_framebuffer_request framebuffer_request = {
    .id = LIMINE_FRAMEBUFFER_REQUEST,
    .revision = 0
};

volatile uint32_t* framebuffer = NULL;
volatile struct limine_framebuffer* this_framebuffer = NULL;

void initialize_graphics()
{
    if (framebuffer_request.response == NULL || framebuffer_request.response->framebuffer_count < 1)
    {
        for (;;);
    }

    framebuffer = (volatile uint32_t*) framebuffer_request.response->framebuffers[0]->address;
    this_framebuffer = framebuffer_request.response->framebuffers[0];
}

static uint32_t calculate_color32(char r, char g, char b)
{
    // Assuming this_framebuffer is a pointer to a structure with the required mask size and shift values.
    uint8_t red_mask_size = this_framebuffer->red_mask_size;
    uint8_t red_mask_shift = this_framebuffer->red_mask_shift;
    uint8_t green_mask_size = this_framebuffer->green_mask_size;
    uint8_t green_mask_shift = this_framebuffer->green_mask_shift;
    uint8_t blue_mask_size = this_framebuffer->blue_mask_size;
    uint8_t blue_mask_shift = this_framebuffer->blue_mask_shift;

    // Calculate the full 8-bit values for R, G, and B components
    uint8_t fullRed = (r << (8 - red_mask_size)) | (r >> red_mask_shift);
    uint8_t fullGreen = (g << (8 - green_mask_size)) | (g >> green_mask_shift);
    uint8_t fullBlue = (b << (8 - blue_mask_size)) | (b >> blue_mask_shift);

    // Combine the R, G, and B values to obtain the final color value
    uint32_t color = (fullRed << 16) | (fullGreen << 8) | fullBlue;

    return color;
}

Color convert_to_rgb(uint32_t color)
{
    Color extracted_color;

    // Extract the R, G, and B components based on mask size and shift values
    uint8_t red = (uint8_t)((color >> this_framebuffer->red_mask_shift) & ((1 << this_framebuffer->red_mask_size) - 1));
    uint8_t green = (uint8_t)((color >> this_framebuffer->green_mask_shift) & ((1 << this_framebuffer->green_mask_size) - 1));
    uint8_t blue = (uint8_t)((color >> this_framebuffer->blue_mask_shift) & ((1 << this_framebuffer->blue_mask_size) - 1));

    // Extend the components to 8-bit range
    red = (red << (8 - this_framebuffer->red_mask_size)) | (red >> (2 * this_framebuffer->red_mask_size - 8));
    green = (green << (8 - this_framebuffer->green_mask_size)) | (green >> (2 * this_framebuffer->green_mask_size - 8));
    blue = (blue << (8 - this_framebuffer->blue_mask_size)) | (blue >> (2 * this_framebuffer->blue_mask_size - 8));

    // Assign the extracted components to the Color structure
    extracted_color.r = red;
    extracted_color.g = green;
    extracted_color.b = blue;

    return extracted_color;
}

uint32_t get_pixel(unsigned int x, unsigned int y)
{
    if (x >= this_framebuffer->width || y >= this_framebuffer->height)
    {
        return;
    }

    return framebuffer[y*this_framebuffer->width + x];
}

void put_pixel(
    unsigned int x,
    unsigned int y,
    unsigned char r,
    unsigned char g,
    unsigned char b
)
{
    if (x >= this_framebuffer->width || y >= this_framebuffer->height)
    {
        return; // Coordinates are out of bounds
    }

    uint32_t color32 = calculate_color32(r, g, b); // Construct 32-bit color

    uint32_t framebuffer_idx = y * this_framebuffer->width + x;
    framebuffer[framebuffer_idx] = color32;
}



void put_rect(
    unsigned int x,
    unsigned int y,
    unsigned int w,
    unsigned int h,
    unsigned char r,
    unsigned char g,
    unsigned char b
)
{
    for (unsigned int i = 0; w > i; i++)
    {
        for (unsigned int j = 0; h > j; j++)
        {
            put_pixel(x + i, y + j, r, g, b);
        }
    }
}

void put_line(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2, unsigned char r, unsigned char g, unsigned char b) {
    unsigned int dx = abs((int)x2 - (int)x1);
    unsigned int dy = abs((int)y2 - (int)y1);
    unsigned int sx, sy;
    
    if (x1 < x2) {
        sx = 1;
    } else {
        sx = -1;
    }

    if (y1 < y2) {
        sy = 1;
    } else {
        sy = -1;
    }

    unsigned int err = dx - dy;
    unsigned int x = x1, y = y1;

    while (1) {
        put_pixel(x, y, r, g, b);

        if (x == x2 && y == y2) {
            break;
        }

        unsigned int e2 = 2 * err;
        if (e2 > -dy) {
            err -= dy;
            x += sx;
        }
        if (e2 < dx) {
            err += dx;
            y += sy;
        }
    }
}

void put_circle(unsigned int centerX, unsigned int centerY, unsigned int radius, unsigned char r, unsigned char g, unsigned char b) {
    int x = radius;
    int y = 0;
    int radiusError = 1 - x;

    while (x >= y) {
        put_pixel(centerX + x, centerY + y, r, g, b);
        put_pixel(centerX - x, centerY + y, r, g, b);
        put_pixel(centerX + x, centerY - y, r, g, b);
        put_pixel(centerX - x, centerY - y, r, g, b);
        put_pixel(centerX + y, centerY + x, r, g, b);
        put_pixel(centerX - y, centerY + x, r, g, b);
        put_pixel(centerX + y, centerY - x, r, g, b);
        put_pixel(centerX - y, centerY - x, r, g, b);

        y++;

        if (radiusError < 0) {
            radiusError += 2 * y + 1;
        } else {
            x--;
            radiusError += 2 * (y - x) + 1;
        }
    }
}

void put_filled_circle(unsigned int centerX, unsigned int centerY, unsigned int radius, unsigned char r, unsigned char g, unsigned char b) {
    int x = radius;
    int y = 0;
    int radiusError = 1 - x;

    while (x >= y) {
        for (unsigned int i = centerX - x; i <= centerX + x; i++) {
            put_pixel(i, centerY + y, r, g, b);
            put_pixel(i, centerY - y, r, g, b);
        }

        for (unsigned int i = centerX - y; i <= centerX + y; i++) {
            put_pixel(i, centerY + x, r, g, b);
            put_pixel(i, centerY - x, r, g, b);
        }

        y++;

        if (radiusError < 0) {
            radiusError += 2 * y + 1;
        } else {
            x--;
            radiusError += 2 * (y - x) + 1;
        }
    }
}

void put_bitmap_image_data(char* bitmap_data, unsigned int x, unsigned int y, unsigned int width, unsigned int height)
{
    // Draw the BMP image using put_pixel
    for (unsigned int i = 0; i < height; i++) {
        for (unsigned int j = 0; j < width; j++) {
            unsigned char b = bitmap_data[3 * (i * width + j)];
            unsigned char g = bitmap_data[3 * (i * width + j) + 1];
            unsigned char r = bitmap_data[3 * (i * width + j) + 2];
            put_pixel(x + j, y + height - 1 - i, r, g, b);
        }
    }
}
unsigned char blurred_bitmap_data[1920*1080*3];
void put_bitmap_image_data_gaussian_blur(char* bitmap_data, unsigned int x, unsigned int y, unsigned int width, unsigned int height)
{
    // Gaussian kernel for larger blur without floating-point arithmetic
    int kernel[7][7] = {
        { 1,  1,  2,  2,  2,  1,  1 },
        { 1,  2,  2,  3,  2,  2,  1 },
        { 2,  2,  3,  4,  3,  2,  2 },
        { 2,  3,  4,  5,  4,  3,  2 },
        { 2,  2,  3,  4,  3,  2,  2 },
        { 1,  2,  2,  3,  2,  2,  1 },
        { 1,  1,  2,  2,  2,  1,  1 }
    };

    // Apply Gaussian blur
    for (unsigned int i = 3; i < height - 3; ++i) {
        for (unsigned int j = 3; j < width - 3; ++j) {
            int sumR = 0, sumG = 0, sumB = 0;
            for (int k = -3; k <= 3; ++k) {
                for (int l = -3; l <= 3; ++l) {
                    unsigned char b = bitmap_data[3 * ((i + k) * width + (j + l))];
                    unsigned char g = bitmap_data[3 * ((i + k) * width + (j + l)) + 1];
                    unsigned char r = bitmap_data[3 * ((i + k) * width + (j + l)) + 2];
                    sumR += r * kernel[k + 3][l + 3];
                    sumG += g * kernel[k + 3][l + 3];
                    sumB += b * kernel[k + 3][l + 3];
                }
            }
            blurred_bitmap_data[3 * (i * width + j)] = (unsigned char)(sumB / 140); // Normalizing factor
            blurred_bitmap_data[3 * (i * width + j) + 1] = (unsigned char)(sumG / 140);
            blurred_bitmap_data[3 * (i * width + j) + 2] = (unsigned char)(sumR / 140);
        }
    }

    // Draw the blurred BMP image using put_pixel
    for (unsigned int i = 0; i < height; ++i) {
        for (unsigned int j = 0; j < width; ++j) {
            unsigned char b = blurred_bitmap_data[3 * (i * width + j)];
            unsigned char g = blurred_bitmap_data[3 * (i * width + j) + 1];
            unsigned char r = blurred_bitmap_data[3 * (i * width + j) + 2];
            put_pixel(x + j, y + height - 1 - i, r, g, b);
        }
    }
}

void put_bitmap_image_data_fit_to(char* bitmap_data, unsigned int x, unsigned int y, unsigned int width, unsigned int height, unsigned int stretched_width, unsigned int stretched_height) {
    // Calculate the scaling factors using integer math
    int x_ratio = (width << 16) / stretched_width + 1; // Using 16-bit precision for scaling
    int y_ratio = (height << 16) / stretched_height + 1;

    // Draw the BMP image using put_pixel and stretch if necessary
    for (unsigned int i = 0; i < stretched_height; ++i) {
        for (unsigned int j = 0; j < stretched_width; ++j) {
            // Calculate the corresponding position in the original bitmap_data
            unsigned int orig_x = (j * x_ratio) >> 16; // Back-shift to get original coordinate
            unsigned int orig_y = (i * y_ratio) >> 16;

            if (orig_x < width && orig_y < height) {
                // Get the RGB values from the original bitmap data
                unsigned char b = bitmap_data[3 * (orig_y * width + orig_x)];
                unsigned char g = bitmap_data[3 * (orig_y * width + orig_x) + 1];
                unsigned char r = bitmap_data[3 * (orig_y * width + orig_x) + 2];

                // Draw the pixel at the stretched position
                put_pixel(x + j, y + stretched_height - 1 - i, r, g, b);
            } else {
                // Handle cases where the stretched image is larger than the original
                // For simplicity, fill the extra space with the last pixel value
                unsigned char b = bitmap_data[3 * ((height - 1) * width + (width - 1))];
                unsigned char g = bitmap_data[3 * ((height - 1) * width + (width - 1)) + 1];
                unsigned char r = bitmap_data[3 * ((height - 1) * width + (width - 1)) + 2];

                // Draw the pixel at the stretched position
                put_pixel(x + j, y + stretched_height - 1 - i, r, g, b);
            }
        }
    }
}