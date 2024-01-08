import axios from "axios";
const myAxiosBE = axios.create({
  baseURL: "http://192.168.164.56:8011",
});
export default myAxiosBE;
