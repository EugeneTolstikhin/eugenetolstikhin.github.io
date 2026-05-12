<script setup lang="ts">
import { ExternalLink, GitBranch } from "lucide-vue-next";
import type { GitHubRepo } from "../types/portfolio";

defineProps<{
  repos: GitHubRepo[];
  locale: string;
}>();
</script>

<template>
  <div class="repo-list">
    <a
      v-for="repo in repos"
      :key="repo.url"
      class="repo-row"
      :href="repo.url"
      target="_blank"
      rel="noreferrer"
    >
      <span class="repo-row__icon">
        <GitBranch aria-hidden="true" :size="18" />
      </span>
      <span class="repo-row__body">
        <strong>{{ repo.name }}</strong>
        <span>{{ repo.description[locale] ?? repo.description.en }}</span>
      </span>
      <span class="repo-row__meta">{{ repo.language }} / {{ repo.updatedAt }}</span>
      <ExternalLink aria-hidden="true" :size="16" />
    </a>
  </div>
</template>
