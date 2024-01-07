<template>
  <w-icon
    :class="`text-[${iconColor}] hover:text-[#2ca012] cursor-pointer `"
    :style="{ fontSize: `${iconSize}` }"
    >{{ iconName }}</w-icon
  >
</template>

<script setup>
const props = defineProps({
  iconName: {},
  iconColor: {
    default: "#888",
  },
  iconSize: {
    default: "24px",
  },
});
</script>

<style lang="scss" scoped></style>
