<script setup lang="ts">
import { storeToRefs } from 'pinia'

import { useUserStore } from '~/stores/user';
import { useSiteStore } from '~/stores/site'

import { useAuth0 } from "~/composables/useAuth"

import { InternalNavigateTo } from '~/lib/utils'

const userStore = useUserStore();
const { tryGetInitials, name, email } = storeToRefs(userStore)

const siteStore = useSiteStore();
const { hasMultipleSites } = storeToRefs(siteStore)

const changeApplication = async () => {
    await InternalNavigateTo('/applications');
}

const logout = async () => {
    const auth0 = await useAuth0();
    await auth0.value.logout();
}

const visitPolis = async() => {
    await navigateTo('https://polis.tv', {
        external: true,
        open: {
            target: '_blank'
        }
    })
}

</script>

<template>
    <v-menu location="bottom center">
        <template v-slot:activator="{ props }">
            <v-btn
                icon
                v-bind="props"
            >
                <v-avatar color="surface-variant">
                    <fa-icon
                        v-if="!tryGetInitials" 
                        :icon="['fas', 'user']" 
                        size="lg"
                    />
                    <span
                        v-else
                        class="font-weight-medium"
                        v-text="tryGetInitials"
                    />
                </v-avatar>
            </v-btn>
        </template>
        <v-card>
            <v-card-text>

                <div class="text-left px-4">
                    <h3 v-text="name" />
                    <p class="text-caption mt-1" v-text="email" />
                </div>

                <v-divider class="my-2"/>

                <v-btn 
                    variant="text"
                    rounded="0"
                    block
                    class="menu__button text-capitalize text-left"
                    @click="logout()"
                >
                    <div class="menu__button__icon">
                        <fa-icon
                            :icon="['fas', 'right-from-bracket']" 
                            size="lg"
                        />
                        
                    </div>
                    <div class="menu__button__text">
                        Logout
                    </div>
                </v-btn>

                <v-btn 
                    v-if="hasMultipleSites"
                    variant="text"
                    rounded="0"
                    block
                    class="menu__button text-capitalize text-left"
                    @click="changeApplication()"
                >
                    <div class="menu__button__icon">
                        <fa-icon
                            :icon="['fass', 'swap']" 
                            size="lg"
                        />
                        
                    </div>
                    <div class="menu__button__text">
                        Change Applicaiton
                    </div>
                </v-btn>

                <v-divider class="my-1"/>
                <v-btn 
                    variant="text"
                    rounded="0"
                    block
                    class="menu__button text-capitalize text-left"
                    @click="visitPolis()"
                >
                    <v-img
                        :aspect-ratio="1"
                        class="menu__button__icon"
                        src="/images/site/polis_icon.png" 
                    />
                    <div class="menu__button__text">
                        Polis Lighting<br /> Controller        
                    </div>
                </v-btn>
            </v-card-text>
        </v-card>
    </v-menu>
</template>

<style scoped>
.menu__button {
    height:52px !important;
}
.menu__button > :deep(.v-btn__content) {
    width: 100%;
}
.menu__button__icon {
    text-align:center;
    flex-shrink: 1;
    flex-grow: 0;
    width: 35px;
}
.menu__button__text {
    flex: 1 1 100%;
    margin-left: 16px !important;
}
</style>