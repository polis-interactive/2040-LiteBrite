// https://nuxt.com/docs/api/configuration/nuxt-config

import { aliases, fa } from 'vuetify/iconsets/fa'

export default defineNuxtConfig({
  modules: [
    '@invictus.codes/nuxt-vuetify',
    '@pinia/nuxt'
  ],
  css: [
    '@fortawesome/fontawesome-svg-core/styles.css'
  ],
  imports: {
    dirs: ['lib']
  },
  typescript: {
    tsConfig: {
      typeRoots: [
        "./node_modules/@types",
        "./node_modules/vuetify/lib/types"
      ]
    }
  },
  ssr: true,
  nitro: {
    compressPublicAssets: {
      gzip: true,
      brotli: false
    }
  },
  vuetify: {
    vuetifyOptions: {
      theme: {
        defaultTheme: 'dark'
      },
      icons: {
        defaultSet: 'fa',
        aliases,
        sets: {
          fa,
        }
      }
    },
    moduleOptions: {
      /* nuxt-vuetify module options */
      treeshaking: true,
      /* vite-plugin-vuetify options */
      styles: true,
      autoImport: true,
      useVuetifyLabs: false,
    }
  },
  devtools: { enabled: true }
})
