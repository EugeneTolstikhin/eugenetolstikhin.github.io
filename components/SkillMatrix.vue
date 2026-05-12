<script setup lang="ts">
import type { SkillGroup } from "../types/portfolio";

defineProps<{
  groups: SkillGroup[];
  locale: string;
}>();
</script>

<template>
  <div class="skill-matrix">
    <article
      v-for="group in groups"
      :key="group.title"
      class="skill-panel"
      :class="`skill-panel--${group.accent}`"
    >
      <h3>{{ group.title[locale] ?? group.title.en }}</h3>
      <p>{{ group.summary[locale] ?? group.summary.en }}</p>
      <div class="tag-row" :aria-label="`${group.title[locale] ?? group.title.en} skills`">
        <span v-for="item in group.items" :key="item" class="tag">{{ item }}</span>
      </div>
    </article>
  </div>
</template>
