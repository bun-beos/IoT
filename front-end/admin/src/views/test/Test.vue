<template>
  <div>
    <EmployeeForm v-show="showForm" @onClose="closeForm"></EmployeeForm>
    <DButton @click="openForm">Show form</DButton>
  </div>
</template>

<script setup>
import { ref } from "vue";
import EmployeeForm from "../employee/EmployeeForm.vue";
const value = ref("Xin chào tôi là Đăng");
const data = ref([{ label: 10 }, { label: 20 }, { label: 50 }, { label: 100 }]);
const checked = ref(false);
const employeeFormData = ref({});
const showForm = ref(false);
function checkedCheckbox() {
  checked.value = !checked.value;
}

function openForm() {
  showForm.value = true;
}

function closeForm() {
  showForm.value = false;
}
function toggleEmployeeForm() {
  showForm.value = !showForm.value;
}
</script>

<style lang="scss" scoped></style>
