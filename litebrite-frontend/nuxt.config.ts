// https://nuxt.com/docs/api/configuration/nuxt-config

import { aliases, fa } from 'vuetify/iconsets/fa'

const Auth0DevConfig = {
  domain: "polis-auth-dev.us.auth0.com",
  clientId: "5J8Kg0rSQ6PsyBoSZ8KRg2uLEa5sPK4D",
  cacheLocation: 'localstorage',
  useRefreshTokens: true,
  authorizationParams: {
    redirect_uri: "http://localhost:3000/applications",
    audience: "http://localhost:8000"
  }
}

const Auth0ProdConfig = {
  domain: "polis-auth.us.auth0.com",
  clientId: "J11vLH9vHEJ9sMdSWWitYm9HjVW3U8V5",
  cacheLocation: 'localstorage',
  useRefreshTokens: true,
  authorizationParams: {
    redirect_uri: "https://lighting.polis.tv/applications",
    audience: "https://lighting.polis.tv/api"
  }

}

const isDev = process.env.NODE_ENV === "development";

export default defineNuxtConfig({
  modules: [
    '@invictus.codes/nuxt-vuetify',
    '@pinia/nuxt',
    '@pinia-plugin-persistedstate/nuxt',
  ],
  piniaPersistedstate: {
    storage: 'localStorage',
    debug: isDev
  },
  css: [
    '@fortawesome/fontawesome-svg-core/styles.css'
  ],
  imports: {
    dirs: ['lib']
  },
  app: {
    head: {
      link: [{ rel: 'icon', type: 'image/x-icon', href: '/images/site/favicon.ico'}],
      title: 'Polis - Light Controller'
    }
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
      selfUrl: isDev ? "http://localhost:3000/" : "https://lighting.polis.tv",
      apiUrl: isDev ? "http://localhost:8000/" : "/api",
      auth0Config: isDev ? Auth0DevConfig : Auth0ProdConfig
    }
  },
  ssr: true,
  nitro: {
    compressPublicAssets: {
      gzip: true,
      brotli: false
    },
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
  devtools: { enabled: isDev },
})
