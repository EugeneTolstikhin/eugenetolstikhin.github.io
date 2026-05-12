export default defineNuxtConfig({
  compatibilityDate: "2026-05-12",
  devtools: { enabled: false },
  css: ["~/assets/css/main.css"],
  app: {
    head: {
      title: "Ievgen Tolstikhin - Engineering Library",
      htmlAttrs: {
        lang: "en",
      },
      meta: [
        { name: "viewport", content: "width=device-width, initial-scale=1" },
        {
          name: "description",
          content:
            "A static library of Ievgen Tolstikhin's skills, technologies, engineering domains, and test projects.",
        },
        { property: "og:title", content: "Ievgen Tolstikhin - Engineering Library" },
        {
          property: "og:description",
          content:
            "Project evidence, technology map, and engineering leadership signals for senior and lead software roles.",
        },
        { property: "og:type", content: "website" },
      ],
      link: [{ rel: "icon", href: "/favicon.svg" }],
    },
  },
  nitro: {
    prerender: {
      crawlLinks: true,
      routes: ["/"],
    },
  },
  vite: {
    optimizeDeps: {
      include: ["lucide-vue-next"],
    },
  },
});
