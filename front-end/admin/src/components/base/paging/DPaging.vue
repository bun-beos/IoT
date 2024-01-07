<template>
  <div
    class="h-[60px] w-[100%] flex items-center justify-between pt-[10px] pb-[10px] bg-[#f5f5f5]"
  >
    <div class="page-count__box flex items-center ml-[10px]">
      {{ resourceLanguage.TotalRecords }}:
      <h1 class="font-bold text-[16px] ml-[4px]">{{ pageCount }}</h1>
    </div>
    <div class="flex">
      <div class="mr-[16px]">
        <!-- Select Box -->
        <!-- <DMenu
          :datas="listPageOptions"
          :initValue="initOption"
          menuPostion="bottom: 0px"
        >
          <template #menu_target>
            <DComboBox></DComboBox>
          </template>
        </DMenu> -->

        <div class="w-[80px]">
          <DComboBox
            :initValue="initOption"
            :listOptions="listPageOptions"
            menuPosition="bottom-[40px]"
            :disabled="true"
            @onOptionChanged="onPageSizeChanged"
          ></DComboBox>
        </div>
      </div>
      <div class="flex items-center mr-[16px]">
        {{ resourceLanguage.From }}
        <strong class="font-bold mx-[4px]">{{ start }}</strong>
        {{ resourceLanguage.To }}
        <strong class="font-bold mx-[4px]">{{ end }}</strong>
        {{ resourceLanguage.Record }}
      </div>
      <div class="flex items-center">
        <!-- <MISAIcon
          icon-name="angle--left"
          width="40px"
          @click="goToPrevPage"
        ></MISAIcon>
        <MISAIcon
          icon-name="angle--right"
          width="40px"
          @click="goToNextPage"
        ></MISAIcon> -->
        <div class="icons__container">
          <DIcon iconName="mdi mdi-chevron-left" @click="goToPrevPage"></DIcon>
        </div>
        <div class="icons__container">
          <DIcon
            iconName="mdi mdi-chevron-right"
            class="mr-[8px]"
            @click="goToNextPage"
          ></DIcon>
        </div>
      </div>
    </div>
  </div>
</template>

<script setup>
import { onMounted, ref } from "vue";
import { storeToRefs } from "pinia";
import { useHelperStore } from "@/stores/HelperStore";
import resource from "./PagingResource";
const emits = defineEmits([
  "onPageSizeChanged",
  "goToNextPage",
  "goToPrevPage",
  "onPageSizeChanged",
]);
const props = defineProps({
  pageCount: {
    type: Number,
  },
  start: {
    type: Number,
  },
  end: {
    type: Number,
  },
  listPageOptions: {
    type: Array,
  },
  initOption: {},
  pageSize: {},
});
const localPageSizeOptions = ref([]);
const languageStore = useHelperStore();
const { language } = storeToRefs(languageStore);
const resourceLanguage = resource[language.value];

/**
 * Thay đổi số bản ghi trong trang
 * @param {Int} newPageSize
 * Created by: nkdang 2/11/2023
 */
function onPageSizeChanged(newPageSize) {
  emits("onPageSizeChanged", newPageSize);
}

/**
 * Hàm emit sự kiện chuyển sang trang tiếp theo, được xử lý ở component cha chứa table
 * Created by: nkmdang 14/11/2023
 */
function goToNextPage() {
  emits("goToNextPage");
}

/**
 * Hàm emit sự kiện chuyển sang trang trước, được xử lý ở component cha chứa table
 * Created by: nkmdang 14/11/2023
 */
function goToPrevPage() {
  emits("goToPrevPage");
}
</script>

<style scoped>
.page-count__title {
  font-size: 14px !important;
  font-weight: bold !important;
}
</style>
