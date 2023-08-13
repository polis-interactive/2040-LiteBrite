<script setup lang="ts">

</script>

<template>
  <v-layout class="centered-layout">
    <v-main class="centered-main">
      <slot />
    </v-main>
  </v-layout>
</template>

<style scoped>
.centered-layout {
    height: 100vh;
    width: 100vw;
}
.centered-main {
    display: flex;
    justify-content: center;
    align-items: center;
    flex-direction: column;
}

</style>