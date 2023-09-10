// https://nuxt.com/docs/api/configuration/nuxt-config

import { aliases, fa } from 'vuetify/iconsets/fa'

const Auth0DevConfig = {
  domain: "polis-auth-dev.us.auth0.com",
  clientId: "5J8Kg0rSQ6PsyBoSZ8KRg2uLEa5sPK4D",
  authorizationParams: {
    redirect_uri: "http://localhost:3000/applications",
    audience: "http://localhost:8000"
  }
}

const Auth0ProdConfig = {
  domain: "polis-auth.us.auth0.com",
  clientId: "5J8Kg0rSQ6PsyBoSZ8KRg2uLEa5sPK4D",
  authorizationParams: {
    redirect_uri: "https://lighting.polis.tv/applications",
    audience: "https://lighting.polis.tv/api"
  }
}

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
  runtimeConfig: {
    public: {
      selfUrl: process.env.NODE_ENV === "development" ? "http://localhost:3000/" : "https://lighting.polis.tv",
      apiUrl: process.env.NODE_ENV === "development" ? "http://localhost:8000/" : "/api",
      auth0Config: process.env.NODE_ENV === "development" ? Auth0DevConfig : Auth0ProdConfig
    }
  },
  ssr: true,
  nitro: {
    compressPublicAssets: {
      gzip: true,
      brotli: false
    },
    prerender: {
      failOnError: false,
      crawlLinks: false,
      routes: ['/', '/login']
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
  devtools: { enabled: true },
})
