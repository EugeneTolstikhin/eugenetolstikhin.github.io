<script setup lang="ts">
import {
  Code2,
  ExternalLink,
  FilterX,
  Github,
  Languages,
  Linkedin,
  Mail,
  MapPin,
  Search,
  ShieldCheck,
} from "lucide-vue-next";
import { computed, onMounted, ref, watch } from "vue";
import {
  githubRepos,
  githubSnapshot,
  localProjects,
  localeOptions,
  metrics,
  principles,
  profile,
  skillGroups,
  spokenLanguages,
  uiByLocale,
} from "./data/portfolio";
import type {
  GitHubApiRepo,
  GitHubRepo,
  LocalizedList,
  LocalizedText,
  PortfolioProject,
} from "./types/portfolio";

const allValue = "__all";
const locale = ref("en");
const query = ref("");
const selectedCategory = ref(allValue);
const selectedDomain = ref(allValue);
const selectedTechnology = ref(allValue);
const liveRepos = ref<GitHubRepo[]>(githubRepos);
const repoState = ref<"fallback" | "live" | "error">("fallback");
const repoSyncedAt = ref(githubSnapshot.syncedAt);

const copy = computed(() => uiByLocale[locale.value] ?? uiByLocale.en);

const text = (value: LocalizedText) => value[locale.value] ?? value.en;
const list = (value: LocalizedList) => value[locale.value] ?? value.en;

useHead(() => ({
  htmlAttrs: {
    lang: locale.value,
  },
  title:
    locale.value === "de"
      ? "Ievgen Tolstikhin - Engineering Bibliothek"
      : "Ievgen Tolstikhin - Engineering Library",
}));

watch(locale, () => {
  selectedCategory.value = allValue;
  selectedDomain.value = allValue;
  selectedTechnology.value = allValue;
});

const categories = computed(() => [
  { value: allValue, label: copy.value.allLabel },
  ...[...new Set(localProjects.map((project) => text(project.category)))]
    .sort()
    .map((category) => ({ value: category, label: category })),
]);

const domains = computed(() => [
  { value: allValue, label: copy.value.allLabel },
  ...[...new Set(localProjects.flatMap((project) => project.domains))]
    .sort()
    .map((domain) => ({ value: domain, label: domain })),
]);

const technologies = computed(() => [
  { value: allValue, label: copy.value.allLabel },
  ...[...new Set(localProjects.flatMap((project) => project.technologies))]
    .sort()
    .map((technology) => ({ value: technology, label: technology })),
]);

const filteredProjects = computed(() => {
  const normalizedQuery = query.value.trim().toLowerCase();

  return localProjects.filter((project) => {
    const category = text(project.category);
    const summary = text(project.summary);
    const evidence = list(project.evidence);
    const matchesCategory =
      selectedCategory.value === allValue || category === selectedCategory.value;
    const matchesDomain =
      selectedDomain.value === allValue || project.domains.includes(selectedDomain.value);
    const matchesTechnology =
      selectedTechnology.value === allValue ||
      project.technologies.includes(selectedTechnology.value);
    const searchable = [
      project.name,
      category,
      summary,
      ...evidence,
      ...project.technologies,
      ...project.domains,
    ]
      .join(" ")
      .toLowerCase();

    return (
      matchesCategory &&
      matchesDomain &&
      matchesTechnology &&
      (!normalizedQuery || searchable.includes(normalizedQuery))
    );
  });
});

const displayedMetrics = computed(() =>
  metrics.map((metric) =>
    metric.label.en === "public GitHub repositories"
      ? { ...metric, value: String(liveRepos.value.length) }
      : metric,
  ),
);

const visibleRepos = computed(() => liveRepos.value);

const repoStatus = computed(() => {
  if (locale.value === "de") {
    return repoState.value === "live"
      ? `${visibleRepos.value.length} Repos / live von GitHub`
      : `${visibleRepos.value.length} Repos / Fallback ${repoSyncedAt.value}`;
  }

  return repoState.value === "live"
    ? `${visibleRepos.value.length} repos / live from GitHub`
    : `${visibleRepos.value.length} repos / fallback ${repoSyncedAt.value}`;
});

const hasActiveFilters = computed(
  () =>
    query.value ||
    selectedCategory.value !== allValue ||
    selectedDomain.value !== allValue ||
    selectedTechnology.value !== allValue,
);

function setLocale(event: Event) {
  const target = event.target as HTMLSelectElement;
  locale.value = target.value;
}

function projectOriginLabel(project: PortfolioProject) {
  if (project.origin === "education") {
    return locale.value === "de" ? "Udacity-Ausbildung" : "Udacity education";
  }

  if (project.origin === "github") {
    return locale.value === "de" ? "GitHub-Projekt" : "GitHub project";
  }

  return locale.value === "de" ? "Lokales Projekt" : "Local project";
}

function projectSourceLabel(project: PortfolioProject) {
  return project.origin === "local"
    ? locale.value === "de"
      ? "Lokale Quelle"
      : "Local source"
    : locale.value === "de"
      ? "GitHub-Quelle"
      : "GitHub source";
}

function toDate(value: string) {
  return value.slice(0, 10);
}

function repoDescription(repo: GitHubApiRepo, fallback?: GitHubRepo) {
  const description = repo.description?.trim();

  if (description) {
    return { en: description, de: description };
  }

  return (
    fallback?.description ?? {
      en: "Public repository.",
      de: "Oeffentliches Repository.",
    }
  );
}

function mapGitHubRepo(repo: GitHubApiRepo): GitHubRepo {
  const fallback = githubRepos.find((item) => item.name === repo.name);

  return {
    name: repo.name,
    description: repoDescription(repo, fallback),
    url: repo.html_url,
    language: repo.language ?? fallback?.language ?? "Not classified",
    topics: repo.topics ?? [],
    stars: repo.stargazers_count,
    forks: repo.forks_count,
    updatedAt: toDate(repo.updated_at),
    pushedAt: toDate(repo.pushed_at),
  };
}

onMounted(async () => {
  try {
    const response = await fetch(githubSnapshot.source, {
      headers: {
        Accept: "application/vnd.github+json",
      },
    });

    if (!response.ok) {
      throw new Error(`GitHub API returned ${response.status}`);
    }

    const repos = (await response.json()) as GitHubApiRepo[];
    liveRepos.value = repos.filter((repo) => !repo.archived && !repo.fork).map(mapGitHubRepo);
    repoState.value = "live";
    repoSyncedAt.value = new Date().toISOString().slice(0, 10);
  } catch {
    repoState.value = "error";
  }
});

function resetFilters() {
  query.value = "";
  selectedCategory.value = allValue;
  selectedDomain.value = allValue;
  selectedTechnology.value = allValue;
}
</script>

<template>
  <div class="app-shell">
    <aside class="profile-rail" aria-label="Profile summary">
      <div class="locale-switch" aria-label="Site language">
        <Languages aria-hidden="true" :size="18" />
        <select :value="locale" @change="setLocale">
          <option v-for="option in localeOptions" :key="option.code" :value="option.code">
            {{ option.label }}
          </option>
        </select>
      </div>

      <div class="profile-rail__identity">
        <div class="identity-mark" aria-hidden="true">IT</div>
        <div>
          <p class="eyebrow">Engineering library</p>
          <h1>{{ profile.name }}</h1>
          <p>{{ text(profile.title) }}</p>
        </div>
      </div>

      <p class="profile-rail__summary">{{ text(profile.summary) }}</p>

      <div class="contact-stack">
        <a :href="`https://www.google.com/maps/search/?api=1&query=${encodeURIComponent(profile.location)}`">
          <MapPin aria-hidden="true" :size="18" />
          {{ profile.location }}
        </a>
        <a href="mailto:etolstikhin@gmail.com">
          <Mail aria-hidden="true" :size="18" />
          etolstikhin@gmail.com
        </a>
        <a href="https://www.linkedin.com/in/ievgen-tolstikhin/" target="_blank" rel="noreferrer">
          <Linkedin aria-hidden="true" :size="18" />
          LinkedIn
        </a>
        <a href="https://www.xing.com/profile/Ievgen_Tolstikhin" target="_blank" rel="noreferrer">
          <ExternalLink aria-hidden="true" :size="18" />
          Xing
        </a>
        <a href="https://github.com/EugeneTolstikhin" target="_blank" rel="noreferrer">
          <Github aria-hidden="true" :size="18" />
          GitHub
        </a>
      </div>

      <dl class="metric-grid">
        <div v-for="metric in displayedMetrics" :key="metric.label.en">
          <dt>{{ metric.value }}</dt>
          <dd>{{ text(metric.label) }}</dd>
        </div>
      </dl>

      <div class="language-strip" aria-label="Spoken languages">
        <span v-for="language in spokenLanguages" :key="language.language.en">
          {{ text(language.language) }} / {{ language.level }}
        </span>
      </div>
    </aside>

    <main class="library-main">
      <section class="workspace-header" aria-labelledby="library-title">
        <div>
          <p class="eyebrow">{{ copy.eyebrow }}</p>
          <h2 id="library-title">{{ copy.headline }}</h2>
        </div>
        <div class="principle-strip" aria-label="Portfolio principles">
          <article v-for="principle in principles" :key="text(principle.title)">
            <ShieldCheck aria-hidden="true" :size="18" />
            <div>
              <strong>{{ text(principle.title) }}</strong>
              <span>{{ text(principle.body) }}</span>
            </div>
          </article>
        </div>
      </section>

      <section class="library-controls" aria-label="Project filters">
        <label class="search-box">
          <span>{{ copy.searchLabel }}</span>
          <Search aria-hidden="true" :size="19" />
          <input v-model="query" type="search" :placeholder="copy.searchPlaceholder" />
        </label>

        <label>
          <span>{{ copy.categoryLabel }}</span>
          <select v-model="selectedCategory">
            <option v-for="category in categories" :key="category.value" :value="category.value">
              {{ category.label }}
            </option>
          </select>
        </label>

        <label>
          <span>{{ copy.domainLabel }}</span>
          <select v-model="selectedDomain">
            <option v-for="domain in domains" :key="domain.value" :value="domain.value">
              {{ domain.label }}
            </option>
          </select>
        </label>

        <label>
          <span>{{ copy.technologyLabel }}</span>
          <select v-model="selectedTechnology">
            <option
              v-for="technology in technologies"
              :key="technology.value"
              :value="technology.value"
            >
              {{ technology.label }}
            </option>
          </select>
        </label>

        <button type="button" :disabled="!hasActiveFilters" @click="resetFilters">
          <FilterX aria-hidden="true" :size="18" />
          {{ copy.resetLabel }}
        </button>
      </section>

      <section class="content-section" aria-labelledby="projects-title">
        <div class="section-heading">
          <div>
            <p class="eyebrow">{{ copy.localEvidenceEyebrow }}</p>
            <h2 id="projects-title">{{ copy.projectsTitle }}</h2>
          </div>
          <span>{{ copy.projectsCount(filteredProjects.length, localProjects.length) }}</span>
        </div>

        <div class="project-grid">
          <ProjectCard
            v-for="project in filteredProjects"
            :key="project.slug"
            :project="project"
            :locale="locale"
            :origin-label="projectOriginLabel(project)"
            :source-label="projectSourceLabel(project)"
          />
        </div>

        <p v-if="filteredProjects.length === 0" class="empty-state">
          {{ copy.noProjects }}
        </p>
      </section>

      <section class="content-section" aria-labelledby="skills-title">
        <div class="section-heading">
          <div>
            <p class="eyebrow">{{ copy.skillsEyebrow }}</p>
            <h2 id="skills-title">{{ copy.skillsTitle }}</h2>
          </div>
          <span>{{ copy.skillsCount(technologies.length - 1) }}</span>
        </div>
        <SkillMatrix :groups="skillGroups" :locale="locale" />
      </section>

      <section class="content-section repo-section" aria-labelledby="github-title">
        <div class="section-heading">
          <div>
            <p class="eyebrow">{{ copy.repoEyebrow }}</p>
            <h2 id="github-title">{{ copy.repoTitle }}</h2>
          </div>
          <span>{{ repoStatus }}</span>
        </div>
        <GitHubRepoList :repos="visibleRepos" :locale="locale" />
        <a class="source-link" :href="githubSnapshot.source" target="_blank" rel="noreferrer">
          <Code2 aria-hidden="true" :size="17" />
          {{ copy.sourceSnapshot }}
        </a>
      </section>
    </main>
  </div>
</template>
