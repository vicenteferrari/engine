//
// Created by vfs on 5/28/2020.
//

void Vec3::blend(float alpha, Vec3 *blended, Vec3 *previous, Vec3 *current) {
    blended->x = current->x * alpha + previous->x * (1.0f - alpha);
    blended->y = current->y * alpha + previous->y * (1.0f - alpha);
    blended->z = current->z * alpha + previous->z * (1.0f - alpha);
}

float fclamp(float x, float upper, float lower) {
    return fmin(upper, fmax(x, lower));
}
