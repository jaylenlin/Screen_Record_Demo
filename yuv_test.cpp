#define UB -128 /* max(-128, round(-2.018 * 64)) */
#define UG 25 /* round(0.391 * 64) */
#define VG 52 /* round(0.813 * 64) */
#define VR -102 /* round(-1.596 * 64) */

#define BB (UB * 128            + YGB)
#define BG (UG * 128 + VG * 128 + YGB)
#define BR            (VR * 128 + YGB)

typedef unsigned char sd_uint8;
typedef unsigned long sd_uint32;
typedef signed long sd_int32;

sd_int32 clamp0(sd_int32 v) { return ((-(v) >> 31) & (v)); }
sd_int32 clamp255(sd_int32 v) { return (((255 - (v)) >> 31) | (v)) & 255; }
sd_uint32 Clamp(sd_int32 val) { int v = clamp0(val); return (sd_uint32)(clamp255(v)); }

void YuvPixel(
    sd_uint8 y,
    sd_uint8 u,
    sd_uint8 v,
    sd_uint8* b,
    sd_uint8* g,
    sd_uint8* r
    ){
    sd_uint32 y1 = (sd_uint32)(y * 0x0101 * YG) >> 16;
    *r = Clamp((sd_int32)(-(u * UB) + y1 + BB) >> 6);
    *g = Clamp((sd_int32)(-(v * VG + u * UG) + y1 + BG) >> 6);
    *b = Clamp((sd_int32)(-(v * VR) + y1 + BR) >> 6);
}
int main(){
    sd_uint8 r,g,b;
    sd_uint8 y, u, v;


    return 0;
}
