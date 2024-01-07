<template>
  <div class="bv__container">
    <div class="h-[36px] flex justify-between items-center">
      <slot name="bv-title"></slot>
    </div>

    <div class="bv__body">
      <slot name="bv-body"> </slot>
    </div>
  </div>
</template>

<script setup></script>

<style lang="scss">
.bv__container {
  padding: 24px;
  background-color: #e6e6e6;
  height: calc(100vh - 48px);
}

.bv__body {
  height: calc(100% - 60px);
  border-radius: 4px;
  background-color: #fff;
  margin-top: 24px;
  overflow: hidden;
}

h1 {
  font-weight: bold;
  font-size: 20px;
}
</style>
