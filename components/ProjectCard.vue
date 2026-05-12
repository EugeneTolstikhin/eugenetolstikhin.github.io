<script setup lang="ts">
import { ExternalLink, FolderGit2 } from "lucide-vue-next";
import { computed } from "vue";
import type { PortfolioProject } from "../types/portfolio";

const props = defineProps<{
  project: PortfolioProject;
  locale: string;
  originLabel: string;
  sourceLabel: string;
}>();

const category = computed(() => props.project.category[props.locale] ?? props.project.category.en);
const status = computed(() => props.project.status[props.locale] ?? props.project.status.en);
const summary = computed(() => props.project.summary[props.locale] ?? props.project.summary.en);
const evidence = computed(() => props.project.evidence[props.locale] ?? props.project.evidence.en);
</script>

<template>
  <article class="project-card">
    <div class="project-card__topline">
      <span class="project-card__origin">{{ originLabel }}</span>
      <span class="project-card__status">{{ status }}</span>
    </div>

    <div class="project-card__header">
      <FolderGit2 aria-hidden="true" :size="22" />
      <div>
        <h3>{{ project.name }}</h3>
        <p>{{ category }}</p>
      </div>
    </div>

    <p class="project-card__summary">{{ summary }}</p>

    <ul class="project-card__evidence" :aria-label="`${project.name} evidence`">
      <li v-for="item in evidence" :key="item">{{ item }}</li>
    </ul>

    <div class="tag-row" :aria-label="`${project.name} technologies`">
      <span v-for="tech in project.technologies" :key="tech" class="tag">{{ tech }}</span>
    </div>

    <div class="project-card__links">
      <a
        v-for="link in project.links"
        :key="link.href"
        :href="link.href"
        target="_blank"
        rel="noreferrer"
      >
        {{ sourceLabel }}
        <ExternalLink aria-hidden="true" :size="16" />
      </a>
    </div>
  </article>
</template>
