<template>
  <div class="relative">
    <div>
      <slot
        name="menu_target"
        @click="activeMenu"
        @keydown.down="nextItem"
        @keydown.up="prevItem"
        @keydown.enter="selectOption(datas[hoverIndexOption - 1].label)"
      ></slot>
    </div>
    <div class="droplist__container" :class="menuPosition">
      <w-list :items="datas" v-model="inputValue">
        <template #item="{ item, index }">
          <div
            class="droplist__item"
            :class="{
              'item--selected':
                item.label == inputValue || hoverIndexOption == index,
            }"
            @click="selectOption(item.label)"
            @mouseenter="hoverIndexOption = index"
          >
            <slot name="menu_item" :data="item">
              {{ item.label }}
            </slot>
          </div>
        </template>
      </w-list>
    </div>
  </div>
</template>

<script setup>
import { ref } from "vue";
const emits = defineEmits(["selectOption"]);
const props = defineProps({
  datas: {},
  menuPosition: {
    type: String,
  },
  initValue: {},
});

const inputValue = ref(props.initValue);
const showList = ref(false);
const hoverIndexOption = ref(0);
/**
 * Hàm chọn lựa chọn bên trong list
 * @param {String} option
 *
 * Created by: nkdang (12/12/2023)
 */
function selectOption(option) {
  inputValue.value = option;
  showList.value = false;
}

/**
 * Hàm xử lý sự kiện bấm phím xuống trong khi đang focus vào thẻ input
 * Created by: nkdang 12/12/2023
 */
function nextItem() {
  if (hoverIndexOption.value < props.datas.length) {
    hoverIndexOption.value = hoverIndexOption.value + 1;
  } else {
    hoverIndexOption.value = 1;
  }
}

/**
 * Hàm xử lý sự kiện bấm phím lên trong khi đang focus vào thẻ input
 * Created by: nkdang 12/12/2023
 */
function prevItem() {
  if (hoverIndexOption.value > 1) {
    hoverIndexOption.value = hoverIndexOption.value - 1;
  } else {
    hoverIndexOption.value = props.listOptions.length;
  }
}

/**
 * Click vào menu để mở list
 * Created by: nkdang (12/12/2023)
 */
function activeMenu() {
  hoverIndexOption.value = 0;
  showList.value = !showList.value;
}
</script>

<style lang="scss" scoped>
/* Drop list trong menu */
.relative:hover .droplist__container {
  display: block;
}
.droplist__container {
  position: absolute;
  display: none;
  padding-top: 8px;
  z-index: 100;
  background: #fff;
}

/* Item trong droplist */
.droplist__item {
  height: 36px;
  padding: 4px;
  padding-left: 6px;
  min-width: 100px;
  background-color: #fff;
  cursor: pointer;
  border-radius: 4px;
  display: flex;
  align-items: center;
}

.droplist__item:hover {
  background-color: var(--item-bg-color);
}

.item--selected {
  background-color: var(--item-bg-color);
}

.w-list {
  box-shadow: 0 3px 1px -2px rgba(0, 0, 0, 0.2), 0 2px 2px 0 rgba(0, 0, 0, 0.15),
    0 1px 5px 0 rgba(0, 0, 0, 0.15);
  border-radius: 4px;
  padding: 8px;
}
</style>
