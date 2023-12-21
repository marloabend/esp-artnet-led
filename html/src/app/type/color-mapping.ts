export const colorMapping = [
  'RGB', 'RBG', 'GRB', 'GBR', 'BRG', 'BGR',
  'WRGB', 'WRBG', 'WGRB', 'WGBR', 'WBRG', 'WBGR',
  'RWGB', 'RWBG', 'RGWB', 'RGBW', 'RBWG', 'RBGW',
  'GWRB', 'GWBR', 'GRWB', 'GRBW', 'GBWR', 'GBRW',
  'BWRG', 'BWGR', 'BRWG', 'BRGW', 'BGWR', 'BGRW',
] as const;

export type ColorMapping = typeof colorMapping[number];
