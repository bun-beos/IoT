<template>
  <div
    class="cb__container flex items-center relative"
    :style="{ width: size, height: size }"
  >
    <!-- <div class="checkbox__box" :style="{ width: size, height: size }"> -->
    <div
      class="icons icon__checkbox bg-[#4caf50] flex items-center justify-center rounded-[4px] overflow-hidden"
      :style="{ width: size, height: size }"
    >
      <w-icon class="text-[#fff] !text-[12px]">{{ icon }}</w-icon>
    </div>
    <div
      :style="{ width: size, height: size }"
      class="input__box"
      :class="{ 'input__box--border': !checked }"
    >
      <div
        class="cb__input"
        :class="{ 'bg-[#fff]': !checked }"
        type="checkbox"
        :style="{ width: size, height: size }"
        @click="checkCheckBox"
      />
    </div>

    <!-- </div> -->
    <div></div>
  </div>
</template>
<script setup>
const emits = defineEmits(["checkCheckBox"]);
const props = defineProps({
  size: {
    type: String,
    default: "18px",
  },
  color: {
    type: String,
  },
  checked: {
    type: Boolean,
  },
  icon: {
    type: String,
    default: "mdi mdi-check",
  },
});

function checkCheckBox() {
  emits("checkCheckBox");
}
</script>
<style>
.input__box {
  position: absolute;
  top: 0;
  left: 0;
  border-radius: 4px;
  overflow: hidden;
}
.cb__input:checked {
  opacity: 0;
}

.input__box--border {
  border: 2px solid #666;
}
</style>
