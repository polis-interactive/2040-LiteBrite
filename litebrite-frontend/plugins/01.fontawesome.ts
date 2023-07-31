
import { library, config } from '@fortawesome/fontawesome-svg-core'
import { FontAwesomeIcon } from '@fortawesome/vue-fontawesome'

import {
    fab,
    faInstagram, faVimeoV, faTiktok
} from '@fortawesome/free-brands-svg-icons'

library.add(
    fab,
    faInstagram, faVimeoV, faTiktok
)

config.autoAddCss = false

export default defineNuxtPlugin((nuxtApp) => {
    nuxtApp.vueApp.component('font-awesome-icon', FontAwesomeIcon)
})
