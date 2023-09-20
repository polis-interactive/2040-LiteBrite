
export interface CRGB {
    r: number,
    g: number,
    b: number
}

export const CRGBFromObject = (object: any): object is CRGB => {
    return 'r' in object && 'g' in object && 'b' in object && !('w' in object)
}

export const CompareCRGB = (a: CRGB, b: CRGB): boolean => {
    return a.r != b.r ||
        a.g != b.g ||
        a.b != b.b
    ;
}

export interface CRGBW {
    r: number,
    g: number,
    b: number,
    w: number
}

export const CRGBWFromObject = (object: any): object is CRGBW => {
    return 'r' in object && 'g' in object && 'b' in object && 'w' in object
}

export const CompareCRGBW = (a: CRGBW, b: CRGBW): boolean => {
    return a.r != b.r ||
        a.g != b.g ||
        a.b != b.b ||
        a.w != b.w
    ;
}