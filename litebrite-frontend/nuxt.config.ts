// https://nuxt.com/docs/api/configuration/nuxt-config

import { aliases, fa } from 'vuetify/iconsets/fa'

export default defineNuxtConfig({
  modules: [
    '@invictus.codes/nuxt-vuetify',
  ],
  css: [
    '@fortawesome/fontawesome-svg-core/styles.css'
  ],
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
