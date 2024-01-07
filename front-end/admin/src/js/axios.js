import axios from "axios";
const myAxiosESP = axios.create({
  baseURL: "http://192.168.0.113",
});
export default myAxiosESP;
