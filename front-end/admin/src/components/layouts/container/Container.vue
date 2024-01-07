<template>
    <div id="container">
        <div class="container-title">{{ pageTitle }}</div>
        <div class="container-content">
            <slot></slot>
        </div>
    </div>
</template>

<script setup>
import { ref, defineProps } from 'vue'
const props = defineProps({
    pageTitle: {
        type: String,
        default: "",
    }
})
</script>

<style lang="css" scoped>
#container {
    width: calc(100vw - 250px);
    height: 100vh;
    background-color: rgb(244, 244, 244);
}

.container-title {
    height: 56px;
    line-height: 56px;
    font-size: 24px;
    padding-left: 16px;    
    border-bottom: 1px solid #333;
}

.container-content {
    height: calc(100% - 56px);
    padding: 28px 20px;
}
</style>