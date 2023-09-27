
import { library, config } from '@fortawesome/fontawesome-svg-core'
import { FontAwesomeIcon } from '@fortawesome/vue-fontawesome'

import {
    fab,
    faInstagram, faVimeoV, faTiktok
} from '@fortawesome/free-brands-svg-icons'

import {
    fas,
    faUser, faRightFromBracket, faTrash, faFloppyDisk, faArrowsRotate
} from '@fortawesome/pro-solid-svg-icons'

import {
    fad,
    faCaretUp
} from '@fortawesome/pro-duotone-svg-icons'

import {
    fass,
    faSwap
} from '@fortawesome/sharp-solid-svg-icons'


library.add(
    fas,
    faUser,
    faRightFromBracket,
    faTrash,
    faFloppyDisk,
    faArrowsRotate
)

library.add(
    fad,
    faCaretUp
)

library.add(
    fass,
    faSwap
)


config.autoAddCss = false

export default defineNuxtPlugin((nuxtApp) => {
    nuxtApp.vueApp.component('fa-icon', FontAwesomeIcon)
})
