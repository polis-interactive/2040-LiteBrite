import { 
    CRGB, CRGBFromObject, CompareCRGB,
    CRGBW, CRGBWFromObject, CompareCRGBW
} from "./color";

const AllDisplayTypes = ['RGB', 'RGBW', 'RGB_WITH_W_INTERPOLATION', 'COLOR_TEMPERATURE'] as const;
type DisplayTypesTuple = typeof AllDisplayTypes;
export type DisplayType = DisplayTypesTuple[number];

export interface DisplayStyle {
    type: DisplayType,
    name: string,
    description: string
}

export const DisplayTypeToStyle: Record<DisplayType, () => DisplayStyle> = {
    'RGB': () => {
        return {
            type: 'RGB',
            name: 'RGB',
            description: 'Plain RGB with W turned off'
        }
    },
    'RGBW': () => {
        return {
            type: 'RGBW',
            name: 'RGBW',
            description: 'Individually controlled RGBW channels'
        }
    },
    'RGB_WITH_W_INTERPOLATION': () => {
        return {
            type: 'RGB_WITH_W_INTERPOLATION',
            name: 'Interpolated RGB',
            description: 'RGB interpolated from W component; may require callibration'
        }
    },
    'COLOR_TEMPERATURE': () => {
        return {
            type: 'COLOR_TEMPERATURE',
            name: 'Color Temperature',
            description: 'Comparable to standard light bulb appearance'
        }
    }
}

export const AllDisplayStyles: DisplayStyle[] = AllDisplayTypes.map(
    (type: DisplayType) => DisplayTypeToStyle[type]()
);

export interface Display {
    style: DisplayStyle,
    rgb_color?: CRGB,
    rgbw_color?: CRGBW,
    color_temperature?: number
}

const AllDisplayValues = ['RGB', 'RGBW', 'COLOR_TEMPERATURE'] as const;
type DisplayValuesTuple = typeof AllDisplayValues;
export type DisplayValue = DisplayValuesTuple[number];

export const GetDisplayValue = (displayType: DisplayType): DisplayValue => {
    switch(displayType) {
        case "RGB":
        case "RGB_WITH_W_INTERPOLATION":
            return 'RGB';
        case "RGBW":
            return 'RGBW';
        case "COLOR_TEMPERATURE":
            return 'COLOR_TEMPERATURE';
    }
}

export const ChangeDisplayType = (newDisplayType: DisplayType, oldDisplay: Display): Display => {
    const newDisplayValue = GetDisplayValue(newDisplayType);
    const oldDisplayValue = GetDisplayValue(oldDisplay.style.type);
    const newDisplay: Display = { style: DisplayTypeToStyle[newDisplayType]() };
    if (newDisplayValue !== oldDisplayValue) {
        switch (newDisplayValue) {
            case "RGB":
                newDisplay.rgb_color = { r: 255, g: 64, b: 127 };
                break;
            case "RGBW":
                newDisplay.rgbw_color = { r: 100, g: 15, b: 70, w: 127 };
                break;
            case "COLOR_TEMPERATURE":
                newDisplay.color_temperature = 4500;
                break;
        }
    } else {
        switch (newDisplayValue) {
            case "RGB":
                newDisplay.rgb_color = { ...oldDisplay.rgb_color as CRGB };
                break;
            case "RGBW":
                newDisplay.rgbw_color = { ...oldDisplay.rgbw_color as CRGBW };
                break;
            case "COLOR_TEMPERATURE":
                newDisplay.color_temperature = oldDisplay.color_temperature;
                break;
        }
    }
    return newDisplay;
} 

export type DisplayComparator = (a: Display, b: Display) => boolean;

export const DisplayComparators: Record<DisplayType, DisplayComparator> = {
    'RGB': ({ rgb_color: a }, { rgb_color: b }) => CompareCRGB(a as CRGB, b as CRGB),
    'RGBW': ({ rgbw_color: a }, { rgbw_color: b }) => CompareCRGBW(a as CRGBW, b as CRGBW),
    'RGB_WITH_W_INTERPOLATION': ({ rgb_color: a }, { rgb_color: b }) => CompareCRGB(a as CRGB, b as CRGB),
    'COLOR_TEMPERATURE': ({ color_temperature: a }, { color_temperature: b }) => a !== b
};


export interface DisplayDTO {
    type?: string,
    rgb_color?: object,
    rgbw_color?: object,
    color_temperature?: number
}

export const DisplayToDTO = (display: Display): DisplayDTO => {
    switch (display.style.type) {
        case "RGB":
            return { type: 'RGB', rgb_color: display.rgb_color };
        case "RGBW":
            return { type: 'RGBW', rgbw_color: display.rgbw_color };
        case "RGB_WITH_W_INTERPOLATION":
            return { type: 'RGB_WITH_W_INTERPOLATION', rgb_color: display.rgb_color };
        case "COLOR_TEMPERATURE":
            return { type: 'COLOR_TEMPERATURE', color_temperature: display.color_temperature };
    }
}

export const DisplayFromDTO = (displayDto: DisplayDTO): Display | null => {
    if (!AllDisplayTypes.includes(displayDto.type as DisplayType || '')) {
        console.error(`lib/domain/display.DisplayFromDTO - unahandled display type: ${displayDto.type} `)
        return null;
    }
    const displayType = displayDto.type as DisplayType;
    const display: Display = { style: DisplayTypeToStyle[displayType]() }
    switch (displayType) {
        case "RGB":
        case "RGB_WITH_W_INTERPOLATION":
            if (!CRGBFromObject(displayDto.rgb_color)) {
                console.error(
                    `lib/domain/display.DisplayFromDTO - rgb_color is not of CRGB type: ${displayDto.rgb_color}`
                );
                return null;
            }
            display.rgb_color = displayDto.rgb_color;
            break;
        case "RGBW":
            if (!CRGBWFromObject(displayDto.rgbw_color)) {
                console.error(
                    `lib/domain/display.DisplayFromDTO - rgbw_color is not of CRGB type: ${displayDto.rgb_color}`
                );
                return null;
            }
            display.rgbw_color = displayDto.rgbw_color;
            break;
        case "COLOR_TEMPERATURE":
            if (typeof displayDto.color_temperature !== "number") {
                console.error(
                    `lib/domain/display.DisplayFromDTO - color_temperature is not of number type: ${displayDto.color_temperature}`
                );
                return null;
            }
            display.color_temperature = displayDto.color_temperature;
            break;
    }
    return display;
}

export interface DisplayState {
    hasLoaded: boolean
    loadingRemote: boolean
    deletingRemote: boolean
    savingRemote: boolean
    localDisplay: Display | null
    remoteDisplay: Display | null
    remoteIsDefault: boolean
}