import axios from "axios";
const myAxiosBE = axios.create({
  baseURL: "http://192.168.0.232:8011",
});
export default myAxiosBE;
