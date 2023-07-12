// https://nuxt.com/docs/api/configuration/nuxt-config
export default defineNuxtConfig({
  ssr: true,
  nitro: {
    compressPublicAssets: {
      gzip: true,
      brotli: false
    }
  },
  devtools: { enabled: true }
})
