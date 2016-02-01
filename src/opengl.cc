#include "opengl.h"

void viewport(int width, int height, float ratio)
{
        int vwidth = width;
        int vheight = (int) (vwidth * (1.0f / ratio));

        if (vheight > height) {
                vheight = height;
                vwidth = (int) (vheight * ratio);
        }

        int vx = (width - vwidth) * 0.5f;
        int vy = (height - vheight) * 0.5f;
        glViewport(vx, vy, vwidth, vheight);
}
