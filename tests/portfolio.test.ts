import { describe, expect, it } from "vitest";
import {
  githubRepos,
  localProjects,
  localeOptions,
  metrics,
  profile,
  skillGroups,
} from "../data/portfolio";

describe("portfolio data", () => {
  it("indexes every portfolio project that should be visible on the site", () => {
    expect(localProjects.map((project) => project.slug).sort()).toEqual([
      "admin-panel",
      "bowling-score",
      "points-generator",
      "small-car-auction",
      "small-image-library",
      "udacity-advanced-lane-lines",
      "udacity-behaviour-cloning",
      "udacity-carnd-capstone",
      "udacity-find-lane-lines",
      "udacity-path-planning",
      "udacity-pid-control",
      "udacity-tensorflow-lab",
      "udacity-traffic-sign-classifier",
    ]);
  });

  it("groups every Udacity repository as an education project", () => {
    const udacityProjects = localProjects.filter((project) => project.origin === "education");

    expect(udacityProjects).toHaveLength(8);
    expect(new Set(udacityProjects.map((project) => project.category.en))).toEqual(
      new Set(["Projects during Udacity education"]),
    );
    expect(
      udacityProjects.every((project) =>
        project.links.some((link) => link.href.includes("github.com/EugeneTolstikhin/udacity-")),
      ),
    ).toBe(true);
  });

  it("keeps GitHub and local project snapshots meaningful for static rendering", () => {
    expect(githubRepos).toHaveLength(11);
    expect(localProjects.every((project) => project.technologies.length > 0)).toBe(true);
    expect(localProjects.every((project) => project.evidence.en.length >= 3)).toBe(true);
    expect(localProjects.every((project) => project.evidence.de.length >= 3)).toBe(true);
  });

  it("preserves the lead engineering signals taken from the CV source", () => {
    const allSkills = skillGroups.flatMap((group) => group.items);

    expect(profile.title.en).toContain("Lead Software Engineer");
    expect(metrics.some((metric) => metric.value === "14+")).toBe(true);
    expect(allSkills).toContain("Nuxt 3/4");
    expect(allSkills).toContain("Domain-Driven Design");
    expect(allSkills).toContain("C++23");
  });

  it("keeps the visible portfolio data localized and application-neutral", () => {
    const serialized = JSON.stringify({
      githubRepos,
      localProjects,
      metrics,
      profile,
      skillGroups,
    });

    expect(localeOptions.map((option) => option.code)).toEqual(["en", "de"]);
    expect(serialized).not.toMatch(/Application variants|CV documents|Recent production signals/);
  });
});
