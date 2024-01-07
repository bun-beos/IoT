<template>
  <w-table
    :headers="headers"
    :items="items"
    :style="style"
    :fixed-headers="fixedHeaders"
    :pagination="pagination"
    resizable-columns
  >
    <template #header-label="{ label, index }">
      <slot name="header-label" :label="label" :index="index">
        {{ label }}
      </slot>
    </template>
    <!-- Không có dữ liệu -->
    <template #no-data>
      <slot name="no-data"> Không có dữ liệu! </slot>
    </template>

    <template #item="{ item, index, select, classes }">
      <slot
        name="item"
        :item="item"
        :index="index"
        :select="select"
        :classes="classes"
      >
        <tr :class="classes" @click="select">
          <td
            v-for="(header, i) in table.headers"
            :key="i"
            :class="`pa4 text-${header.align || 'left'}`"
          >
            {{ item[header.key] || "" }}
          </td>
        </tr>
      </slot>
    </template>

    <template #item-cell.checkBox="{ item, label, header, index }">
      <slot
        name="item-cell.checkBox"
        :item="item"
        :index="index"
        :select="select"
        :classes="classes"
      >
        <input type="checkbox" />
      </slot>
    </template>
  </w-table>
</template>

<script setup>
import { ref } from "vue";
const props = defineProps({
  fixedHeaders: {
    type: Boolean,
    default: false,
  },
  headers: {
    type: Array,
    default: [],
  },
  items: {
    type: Array,
    default: [],
  },
  mobileBreakpoint: {
    type: String,
    default: "700",
  },
  style: {},
  pagination: {
    type: Object,
  },
});
</script>

<style lang="scss" scoped></style>
