<template>
  <w-list :items="items">
    <template #item="{ index }">{{ index }}</template>
  </w-list>
</template>

<script setup>
const props = defineProps({
  items: {},
});
</script>

<style lang="scss" scoped></style>
