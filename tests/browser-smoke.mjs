import { mkdir } from "node:fs/promises";
import { chromium } from "playwright-core";

const url = process.env.PORTFOLIO_URL ?? "http://host.docker.internal:3000";
const screenshotBase = process.env.SCREENSHOT_BASE ?? "test-results/portfolio-smoke";

await mkdir("test-results", { recursive: true });

const browser = await chromium.launch({ args: ["--no-sandbox"] });
const consoleErrors = [];
const failures = [];

async function verifyViewport(name, viewport, shouldExerciseSearch) {
  const page = await browser.newPage({ viewport });

  page.on("console", (message) => {
    if (message.type() === "error") {
      consoleErrors.push(`[${name}] ${message.text()}`);
    }
  });

  page.on("pageerror", (error) => {
    consoleErrors.push(`[${name}] ${error.message}`);
  });

  await page.goto(url, { waitUntil: "networkidle", timeout: 45000 });

  const bodyText = await page.locator("body").innerText();
  const projectCards = await page.locator(".project-card").count();
  const skillPanels = await page.locator(".skill-panel").count();
  const repoRows = await page.locator(".repo-row").count();
  const documentCards = await page.locator(".document-card").count();
  const profileImages = await page.locator(".profile-rail img").count();
  const horizontalOverflow = await page.evaluate(
    () => document.documentElement.scrollWidth - window.innerWidth,
  );

  if (!bodyText.includes("Project library")) {
    failures.push(`[${name}] Missing project library heading.`);
  }

  if (projectCards < 13) {
    failures.push(`[${name}] Expected at least 13 project cards, found ${projectCards}.`);
  }

  if (skillPanels < 5) {
    failures.push(`[${name}] Expected at least 5 skill panels, found ${skillPanels}.`);
  }

  if (repoRows < 11) {
    failures.push(`[${name}] Expected at least 11 GitHub repo rows, found ${repoRows}.`);
  }

  if (documentCards !== 0) {
    failures.push(`[${name}] Application variants should be hidden, found ${documentCards} document cards.`);
  }

  if (bodyText.includes("Application variants") || bodyText.includes("CV documents")) {
    failures.push(`[${name}] Hidden application variants section is still visible.`);
  }

  if (profileImages !== 0) {
    failures.push(`[${name}] Profile photo should not render, found ${profileImages} image(s).`);
  }

  if (bodyText.includes("Recent production signals")) {
    failures.push(`[${name}] Removed production signals section is still visible.`);
  }

  if (bodyText.includes('"en":') || bodyText.includes("[object Object]")) {
    failures.push(`[${name}] Localized object data leaked into visible portfolio UI.`);
  }

  if (horizontalOverflow > 2) {
    failures.push(`[${name}] Horizontal overflow detected: ${horizontalOverflow}px.`);
  }

  if (shouldExerciseSearch) {
    await page.locator('input[type="search"]').fill("auction");
    await page.waitForTimeout(300);
    const filteredText = await page.locator("body").innerText();

    if (!filteredText.includes("Small Car Auction")) {
      failures.push(`[${name}] Search filter did not keep the auction project visible.`);
    }

    await page.locator(".locale-switch select").selectOption("de");
    await page.waitForTimeout(300);
    const germanText = await page.locator("body").innerText();

    if (!germanText.includes("Projektbibliothek")) {
      failures.push(`[${name}] German locale did not switch the project heading.`);
    }
  }

  await page.screenshot({ path: `${screenshotBase}-${name}.png`, fullPage: true });
  await page.close();

  return { name, projectCards, skillPanels, repoRows, documentCards, horizontalOverflow };
}

const desktop = await verifyViewport("desktop", { width: 1440, height: 1100 }, true);
const mobile = await verifyViewport("mobile", { width: 390, height: 900 }, false);

await browser.close();

if (consoleErrors.length > 0) {
  failures.push(`Console errors detected: ${consoleErrors.join(" | ")}`);
}

if (failures.length > 0) {
  console.error(failures.join("\n"));
  process.exit(1);
}

console.log(
  JSON.stringify(
    {
      status: "ok",
      url,
      viewports: [desktop, mobile],
      screenshots: [`${screenshotBase}-desktop.png`, `${screenshotBase}-mobile.png`],
    },
    null,
    2,
  ),
);
