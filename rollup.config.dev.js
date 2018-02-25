import baseConfig from './rollup.config.base';

import { name } from './package.json';

let config = {
  ...baseConfig,
  watch: {
    include: 'src/**/*'
  },
  output: [
    {
      file: `temp/${name}.js`,
      format: 'es',
      name,
      sourcemap: 'inline'
    }
  ]
};

export default config;
