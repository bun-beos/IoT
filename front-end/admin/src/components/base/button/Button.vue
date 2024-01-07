<template>
  <w-button
    :bg-color="bgColor"
    height="36px"
    :class="{ '!border-[#ccc]': bgColor == 'white' }"
  >
    <slot></slot>
  </w-button>
</template>

<script setup>
const props = defineProps({
  bgColor: {
    type: String,
    default: "success",
  },
  route: {
    type: String,
  },
  loading: {
    type: Boolean,
  },
  tooltip: {
    type: String,
  },
});
</script>

<style lang="scss" scoped>
.w-button {
  min-width: 80px;
  border-radius: 4px !important;
}

.button--white {
  border: 1px solid #ccc !important;
}
</style>
