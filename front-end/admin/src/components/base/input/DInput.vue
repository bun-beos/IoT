<template>
  <div class="input__text-box">
    <w-tooltip bottom v-if="tooltip">
      <template #activator="{ on }">
        <div v-if="label" @click="focusInput" class="w-fit font-bold">
          <span
            class="flex items-center w-fit"
            :class="labelStyle"
            :id="idLabel"
          >
            {{ label }}
            <div v-if="required" class="text-[red] ml-[2px]">*</div>
          </span>
        </div>
      </template>
      {{ tooltip }}
    </w-tooltip>
    <div v-if="label && !tooltip" @click="focusInput" class="w-fit font-bold">
      <span class="flex items-center w-fit" :class="labelStyle" :id="idLabel">
        {{ label }}
        <div v-if="required" class="text-[red] ml-[2px]">*</div>
      </span>
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
    >
      <div class="icons__container" v-show="icon?.iconPosition == 'left'">
        <DIcon
          iconSize="20px"
          iconColor="#ccc"
          :iconName="icon?.iconName"
        ></DIcon>
      </div>
      <input
        :tabindex="tabindex"
        ref="inputTextRef"
        :type="type"
        :class="{
          input__disabled: disabled,
          input__text: icon?.iconPosition != 'left',
        }"
        :style="{ height: height, width: '100%' }"
        :disabled="disabled"
        :placeholder="placeholder"
        @focus="focusInput"
        @blur="blurInput"
        @input="emitModelValue"
        v-model="inputValue"
      />
      <div class="icons__container" v-show="icon?.iconPosition == 'right'">
        <DIcon
          iconSize="20px"
          :iconColor="icon?.iconColor || '#ccc'"
          :iconName="icon?.iconName"
        ></DIcon>
      </div>
    </div>
  </div>
</template>

<script setup>
import { ref, computed, onMounted } from "vue";
const emits = defineEmits(["update:modelValue"]);
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
  modelValue: {},
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
  icon: {
    type: Object,
    default: {},
  },
  focus: {
    type: Boolean,
  },
  type: {
    type: String,
    default: "text",
  },
});

defineExpose({
  focusInput,
});
const isActive = ref(false);
const error = computed(() => ({
  isError: false,
  type: "",
}));
const borderColor = ref("#e0e0e0");
const inputTextRef = ref(null);

const isMissingInput = ref(props.isMissedInput);

// Model giá trị thẻ input
const inputValue = ref(props.initValue);

/**
 * Binding dữ liệu với dữ liệu ở component cha
 * Created By: nkdang (24/10/2023)
 */
function emitModelValue() {
  console.log("update model value");
  emits("update:modelValue", inputValue.value);
}

/**
 * Active thẻ input thì đổi màu border và đánh dấu isActive = true
 * Created By: nkmdang 24/10/2023
 */
function activeInput() {
  isActive.value = true;
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
}

function setTooltip(on) {
  if (props.tooltip) {
    return on;
  } else {
    return null;
  }
}

onMounted(() => {
  if (props.focus) {
    setTimeout(() => {
      inputTextRef.value.select();
      inputTextRef.value.focus();
    }, 200);
    // console.log(inputTextRef);
  }
});
</script>

<style scoped>
.container__box {
  border: 1px solid #aaa;
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
</style>
