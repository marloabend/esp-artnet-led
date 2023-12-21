import {ColorMapping} from '../type/color-mapping';

export interface Config {
  pixels: number;
  colorMapping: ColorMapping;
  white: boolean;
  brightness: number;
  hsv: boolean;
  reverse: boolean;
  speed: number;
  split: number;
}
