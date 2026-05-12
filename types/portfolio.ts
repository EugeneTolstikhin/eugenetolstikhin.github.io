export type ContactLink = {
  label: string;
  href: string;
};

export type LocalizedText = Record<string, string>;
export type LocalizedList = Record<string, string[]>;

export type LocaleOption = {
  code: string;
  label: string;
  shortLabel: string;
};

export type ProfileMetric = {
  label: LocalizedText;
  value: string;
};

export type SkillGroup = {
  title: LocalizedText;
  summary: LocalizedText;
  items: string[];
  accent: "teal" | "violet" | "amber" | "green" | "coral";
};

export type PortfolioProject = {
  slug: string;
  name: string;
  origin: "local" | "github" | "education";
  category: LocalizedText;
  status: LocalizedText;
  summary: LocalizedText;
  evidence: LocalizedList;
  technologies: string[];
  domains: string[];
  links: ContactLink[];
};

export type GitHubRepo = {
  name: string;
  description: LocalizedText;
  url: string;
  language: string;
  topics: string[];
  updatedAt: string;
  pushedAt: string;
  stars: number;
  forks: number;
};

export type GitHubApiRepo = {
  name: string;
  description: string | null;
  html_url: string;
  language: string | null;
  topics?: string[];
  stargazers_count: number;
  forks_count: number;
  updated_at: string;
  pushed_at: string;
  archived: boolean;
  fork: boolean;
};

export type Principle = {
  title: LocalizedText;
  body: LocalizedText;
};

export type UiCopy = {
  eyebrow: string;
  headline: string;
  searchLabel: string;
  searchPlaceholder: string;
  categoryLabel: string;
  domainLabel: string;
  technologyLabel: string;
  allLabel: string;
  resetLabel: string;
  localEvidenceEyebrow: string;
  projectsTitle: string;
  projectsCount: (visible: number, total: number) => string;
  noProjects: string;
  skillsEyebrow: string;
  skillsTitle: string;
  skillsCount: (count: number) => string;
  repoEyebrow: string;
  repoTitle: string;
  synced: (date: string) => string;
  sourceSnapshot: string;
};
