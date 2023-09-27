<script setup lang="ts">
import { storeToRefs } from 'pinia';

import { useDisplay } from 'vuetify'

import { useDisplayStore } from '~/stores/display';


const displayStore = useDisplayStore();

const { xs } = useDisplay();

const { 
  remoteIsDefault, hasSaveableChange, displayIsLoading,
  deletingRemote, savingRemote
} = storeToRefs(displayStore)
</script>

<template>
    <v-card-actions>
        <v-tooltip location="top">
            <template v-slot:activator="{ props }">
                <div 
                    v-bind="props"
                    class="mr-4"
                >
                    <v-btn
                        variant="tonal"
                        :size="xs ? 'default' : 'large'"
                        color="error"
                        :disabled="displayIsLoading || remoteIsDefault"
                        :loading="deletingRemote"
                        @click="displayStore.deleteCurrentDisplay"
                    >
                        <div class="action__icon">
                            <fa-icon
                                :icon="['fas', 'trash']" 
                                size="lg"
                            />
                        </div>
                        <div class="action__text">
                            Delete
                        </div>
                    </v-btn>
                </div>
            </template>
            <div class="text-center">
                Removes saved display,<br />returns fixture to default
            </div>
        </v-tooltip>
        <v-tooltip location="top">
            <template v-slot:activator="{ props }">
                <div 
                    v-bind="props"
                >
                    <v-btn
                        variant="tonal"
                        :size="xs ? 'default' : 'large'"
                        :disabled="displayIsLoading || !hasSaveableChange"
                        @click="displayStore.resetCurrentDisplay"
                    >
                        <div class="action__icon">
                            <fa-icon
                                :icon="['fas', 'arrows-rotate']" 
                                size="lg"
                            />
                        </div>
                        <div class="action__text">
                            Reset
                        </div>
                    </v-btn>
                </div>
            </template>
            <div class="text-center">
                Resets local display<br />to the fixture
            </div>
        </v-tooltip>
        <v-spacer class="actions_spacer" />
        <v-tooltip location="top">
            <template v-slot:activator="{ props }">
                <div 
                    v-bind="props"
                >
                    <v-btn
                        variant="tonal"
                        color="success"
                        :size="xs ? 'default' : 'large'"
                        :disabled="displayIsLoading || !hasSaveableChange"
                        :loading="savingRemote"
                        @click="displayStore.saveCurrentDisplay"
                    >
                        <div class="action__icon">
                            <fa-icon
                                :icon="['fas', 'floppy-disk']" 
                                size="lg"
                            />
                        </div>
                        <div class="action__text">
                            Submit
                        </div>
                    </v-btn>
                </div>
            </template>
            <div class="text-center">
                Saves local display<br />to the fixture
            </div>
        </v-tooltip>
    </v-card-actions>
</template>

<style scoped>
.actions_spacer {
    width: 100%;
}
.action__icon {
  margin-right: 0.5rem;
}
</style>
