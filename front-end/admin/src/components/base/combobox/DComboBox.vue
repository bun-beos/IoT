<template>
  <div class="relative w-[100%]">
    <div class="input__text-box" v-click-outside="onClickOutSide">
      <div v-if="label" @click="focusInput">
        <div
          class="flex items-center font-bold"
          :class="labelStyle"
          :id="idLabel"
        >
          {{ label }}
          <div v-if="required" class="text-[red] ml-[2px]">*</div>
        </div>
      </div>
      <div
        class="container__box"
        :class="{
          'input__text--box flex justify-start': true,
          'container__box--active': isActive,
          'border-[#e0e0e0]': disabled,
          'hover:border-[#e0e0e0]': disabled,
        }"
        :style="{ height: height }"
        @click="toggleList"
      >
        <div class="icons__container" v-show="icon?.iconPosition == 'left'">
          <DIcon
            :iconSize="icon?.iconSize"
            :iconColor="icon?.iconColor"
            :iconName="icon?.iconName"
          ></DIcon>
        </div>
        <input
          :tabindex="tabindex"
          ref="inputText"
          type="text"
          :class="{
            input__disabled: disabled,
            input__text: icon?.iconPosition != 'left',
          }"
          :style="{
            height: height,
            width: '100%',
            cursor: `${disabled ? 'pointer' : 'auto'}`,
          }"
          :readonly="disabled"
          :placeholder="placeholder"
          @focus="focusInput"
          @blur="blurInput"
          @input="comboBoxInput"
          @keydown.down="nextItem"
          @keydown.up="prevItem"
          @keydown.enter="
            selectOption(currentListOptions[hoverIndexOption - 1].label)
          "
          v-model="inputValue"
        />
        <div
          class="icons__container cursor-pointer"
          v-show="icon?.iconPosition == 'right'"
        >
          <w-icon :class="`!text-[20px] text-[#000] hover:text-[#2ca012] `"
            >mdi mdi-chevron-down</w-icon
          >
        </div>
      </div>
    </div>
    <div
      v-show="showList"
      :class="menuPosition"
      class="droplist__container bg-[#fff] w-[100%]"
    >
      <w-list
        :items="currentListOptions"
        v-model="selectedOption"
        v-show="currentListOptions"
      >
        <template #item="{ item, index }">
          <div
            class="droplist__item"
            :class="{
              'item--selected':
                item.label == inputValue || index == hoverIndexOption,
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

      <w-list
        :items="[{ label: 'Không có dữ liệu' }]"
        v-show="!currentListOptions"
      >
        <template #item="{ item, index }">
          <div
            class="droplist__item"
            :class="{
              'item--selected': index == hoverIndexOption,
            }"
            @mouseenter="hoverIndexOption = index"
          >
            <slot name="menu_item" :data="item">
              {{ item.label }}
            </slot>
          </div>
        </template></w-list
      >
    </div>
  </div>
</template>

<script setup>
import { ref, computed } from "vue";
const emits = defineEmits(["update:modelValue", "onOptionChanged"]);
const props = defineProps({
  activeColor: {
    type: String,
  },
  height: {
    type: String,
    default: "36px",
  },
  label: {
    type: String,
  },
  labelStyle: {
    type: String,
  },
  modelValue: "",
  required: {
    type: Boolean,
  },
  isMissedInput: {
    type: Boolean,
  },
  disabled: {
    type: Boolean,
  },
  tooltip: {
    type: String,
  },
  idLabel: {
    type: String,
  },
  placeholder: {
    type: String,
  },
  tabindex: {
    type: Number,
  },
  initValue: {},
  listOptions: {
    type: Array,
  },
  icon: {
    type: Object,
    default: {
      iconName: "mdi mdi-triangle-down",
      iconColor: "#ccc",
      iconPosition: "right",
      iconSize: "15px",
    },
  },
  menuPosition: {
    type: String,
  },
});

const selectedOption = ref(props.initValue);

defineExpose({
  focusInput,
});
const isActive = ref(false);
const inputText = ref(null);

const isMissingInput = ref(props.isMissedInput);

// Model giá trị thẻ input
const inputValue = ref(props.initValue);
const showList = ref(false);

// Current ListOptions
const currentListOptions = ref(props.listOptions || []);

// Sự kiện bàn phím trên combobox
const hoverIndexOption = ref(0);
/**
 * Binding dữ liệu với dữ liệu ở component cha
 * Created By: nkdang (24/10/2023)
 */
function emitModelValue() {
  emits("update:modelValue", inputValue.value);
}

/**
 * Blur thẻ input thì đổi màu border và đánh dấu isActive = false
 * Created By: nkmdang 24/10/2023
 */
function blurInput() {
  isActive.value = false;
}

/**
 * Focus vào thẻ input
 * Created By: nkmdang 24/10/2023
 */
function focusInput(e) {
  e.target.select();
  e.target.focus();
  isActive.value = true;
  hoverIndexOption.value = 0;
}

/**
 * Bật tắt drop list
 * Created by: nkdang 12/12/2023
 */
function toggleList() {
  currentListOptions.value = props.listOptions;
  showList.value = !showList.value;
}

/**
 * Hàm chọn lựa chọn bên trong list
 * @param {String} option
 *
 * Created by: nkdang (12/12/2023)
 */
function selectOption(option) {
  emits("update:modelValue");
  emits("onOptionChanged", option);
  inputValue.value = option;
  showList.value = false;
}

/**
 * Hàm xử lý Autocomplete
 * @param {Event} e
 */
function comboBoxInput(e) {
  showList.value = true;
  const newValue = e.target.value;
  currentListOptions.value = props.listOptions.filter((option) =>
    option.label.toString().includes(newValue)
  );
}

/**
 * Hàm xử lý sự kiện bấm phím xuống trong khi đang focus vào thẻ input
 * Created by: nkdang 12/12/2023
 */
function nextItem() {
  if (hoverIndexOption.value < props.listOptions.length) {
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
 * Hàm tắt drop list và unactive combobox khi click ra bên ngoài
 * Created by: nkdang 14/12/2023
 */
function onClickOutSide() {
  showList.value = false;
  isActive.value = false;
}
</script>

<style scoped>
.container__box {
  border: 1px solid #ccc;
  border-radius: 4px;
  overflow: hidden;
}

.container__box:hover {
  border-color: #2ca012;
}

.container__box--active {
  border-color: #2ca012;
}
.input__text--box {
  /* border: 1px solid transparent; */
  border-radius: 4px;
}

.input--error {
  border-color: red !important;
}

.input__disabled {
  background-color: #f4f5f8;
}

/* CSS cho thẻ input */
.input__text {
  padding-left: 8px;
}

.droplist__container {
  position: absolute;
}

/* Item trong droplist */
.droplist__item {
  height: 36px;
  padding: 4px;
  padding-left: 6px;
  width: 100%;
  /* min-width: 80px; */
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
