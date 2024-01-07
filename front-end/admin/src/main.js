import { createApp } from "vue";
import { createPinia } from "pinia";
import "./css/index.css";
import router from "./router/router";
import "@mdi/font/css/materialdesignicons.min.css";
import App from "./App.vue";
// import WaveUI from "wave-ui";
// import "wave-ui/dist/wave-ui.css";
import WaveUI from "wave-ui/src/wave-ui";
import vClickOutside from "click-outside-vue3";
// Các component dùng chung
import Button from "@/components/base/button/Button.vue";
import DCheckBox from "@/components/base/checkbox/DCheckBox.vue";
import Table from "@/components/base/table/Table.vue";
import DInput from "@/components/base/input/DInput.vue";
import DIcon from "@/components/base/icon/DIcon.vue";
import DComboBox from "@/components/base/combobox/DComboBox.vue";
import Menu from "@/components/base/menu/Menu.vue";
import DForm from "@/components/base/form/DForm.vue";
const app = createApp(App);

app.use(WaveUI, {
  /* Some Wave UI options */
});
// Or in Wave UI 2.x:
// new WaveUI(app, { /* Some Wave UI options */ })

// Pinia
app.use(createPinia());
app.use(router);

// V-click-outside
app.use(vClickOutside);

// Register Component
app.component("DButton", Button);
app.component("DTable", Table);
app.component("DInput", DInput);
app.component("DIcon", DIcon);
app.component("DComboBox", DComboBox);
app.component("DMenu", Menu);
app.component("DCheckBox", DCheckBox);
app.component("DForm", DForm);
app.config.errorHandler = (err, vm, info) => {
  // handle error
  // `info` is a Vue-specific error info, e.g. which lifecycle hook
  // the error was found in
  console.log(err + "\n" + info);
  // console.log(vm);
  // console.log(info);
};

app.config.warnHandler = function (msg, vm, trace) {
  // `trace` is the component hierarchy trace
  console.log(msg + "\n" + trace);
  // console.log(vm);
  // console.log(trace);
};

app.mount("#app");
