<template>
  <div id="home">
    <Container :pageTitle="'Home'">
      <div class="home-content">
        <div>
          <div style="font-size: 50px; margin-bottom: 50px;">IoT-Smart Doorbell</div>
          <div style="font-size: 20px;">
            Trần Trung Kiên - 20200304 <br />
            Nguyễn Khánh Minh Đăng - 20204720 <br />
            Lê Văn Tùng - 20204803
          </div>
        </div>
      </div>
    </Container>
  </div>
</template>

<script setup>
import Container from "../../components/layouts/container/Container.vue";
</script>

<style lang="css" scoped>
.home-content {
  display: flex;
  height: 100%;
  align-items: center;
  justify-content: center;
  text-align: center;
}
</style>
