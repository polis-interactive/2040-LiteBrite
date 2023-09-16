
import { library, config } from '@fortawesome/fontawesome-svg-core'
import { FontAwesomeIcon } from '@fortawesome/vue-fontawesome'

import {
    fab,
    faInstagram, faVimeoV, faTiktok
} from '@fortawesome/free-brands-svg-icons'

import {
    fas,
    faUser, faRightFromBracket
} from '@fortawesome/pro-solid-svg-icons'

import {
    fass,
    faSwap
} from '@fortawesome/sharp-solid-svg-icons'

library.add(
    fas,
    faUser,
    faRightFromBracket
)

library.add(
    fass,
    faSwap
)

config.autoAddCss = false

export default defineNuxtPlugin((nuxtApp) => {
    nuxtApp.vueApp.component('fa-icon', FontAwesomeIcon)
})
