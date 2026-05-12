import type {
  ContactLink,
  GitHubRepo,
  LocaleOption,
  PortfolioProject,
  Principle,
  ProfileMetric,
  SkillGroup,
  UiCopy,
} from "../types/portfolio";

export const localeOptions = [
  { code: "en", label: "English", shortLabel: "EN" },
  { code: "de", label: "Deutsch", shortLabel: "DE" },
] satisfies LocaleOption[];

export const uiByLocale: Record<string, UiCopy> = {
  en: {
    eyebrow: "Projects / skills / technologies",
    headline: "A browsable evidence map for senior engineering work.",
    searchLabel: "Search library",
    searchPlaceholder: "Search project, stack, domain...",
    categoryLabel: "Category",
    domainLabel: "Domain",
    technologyLabel: "Technology",
    allLabel: "All",
    resetLabel: "Reset",
    localEvidenceEyebrow: "Project evidence",
    projectsTitle: "Project library",
    projectsCount: (visible, total) => `${visible} of ${total} projects`,
    noProjects: "No projects match the current filters.",
    skillsEyebrow: "Capability index",
    skillsTitle: "Skills and technologies",
    skillsCount: (count) => `${count} project technologies indexed`,
    repoEyebrow: "Public GitHub snapshot",
    repoTitle: "Repository history",
    synced: (date) => `Synced ${date}`,
    sourceSnapshot: "Source API snapshot",
  },
  de: {
    eyebrow: "Projekte / Skills / Technologien",
    headline: "Eine durchsuchbare Evidenzkarte fuer Senior Engineering Arbeit.",
    searchLabel: "Bibliothek durchsuchen",
    searchPlaceholder: "Projekt, Stack oder Domaene suchen...",
    categoryLabel: "Kategorie",
    domainLabel: "Domaene",
    technologyLabel: "Technologie",
    allLabel: "Alle",
    resetLabel: "Zuruecksetzen",
    localEvidenceEyebrow: "Projekt-Evidenz",
    projectsTitle: "Projektbibliothek",
    projectsCount: (visible, total) => `${visible} von ${total} Projekten`,
    noProjects: "Keine Projekte passen zu den aktuellen Filtern.",
    skillsEyebrow: "Kompetenzindex",
    skillsTitle: "Skills und Technologien",
    skillsCount: (count) => `${count} Projekttechnologien indexiert`,
    repoEyebrow: "Oeffentlicher GitHub-Snapshot",
    repoTitle: "Repository-Historie",
    synced: (date) => `Synchronisiert ${date}`,
    sourceSnapshot: "API-Snapshot",
  },
};

export const profile = {
  name: "Ievgen Tolstikhin",
  title: {
    en: "Senior / Lead Software Engineer",
    de: "Senior / Lead Software Engineer",
  },
  location: "Berlin, Germany",
  nationality: {
    en: "German",
    de: "Deutsch",
  },
  summary: {
    en:
      "Software engineer with 14+ years across healthcare SaaS, remote connectivity, automotive, telecom, POS, CAD, and public-sector systems. The strongest signal is the mix: TypeScript/Vue/Nuxt and Node platforms, production-grade backend boundaries, Docker/Kubernetes delivery, and deep C++ systems experience.",
    de:
      "Software Engineer mit 14+ Jahren Erfahrung in Healthcare SaaS, Remote Connectivity, Automotive, Telekommunikation, POS, CAD und Public-Sector-Systemen. Das staerkste Signal ist die Kombination aus TypeScript/Vue/Nuxt, Node-Plattformen, produktionsreifen Backend-Grenzen, Docker/Kubernetes Delivery und tiefer C++ Systemerfahrung.",
  },
  contacts: [
    { label: "Email", href: "mailto:etolstikhin@gmail.com" },
    { label: "LinkedIn", href: "https://www.linkedin.com/in/ievgen-tolstikhin/" },
    { label: "Xing", href: "https://www.xing.com/profile/Ievgen_Tolstikhin" },
    { label: "GitHub", href: "https://github.com/EugeneTolstikhin" },
  ] satisfies ContactLink[],
};

export const metrics = [
  { value: "14+", label: { en: "years engineering", de: "Jahre Engineering" } },
  { value: "20", label: { en: "CI/CD pipelines operated", de: "CI/CD-Pipelines betrieben" } },
  {
    value: "400+",
    label: { en: "automated tests in product delivery", de: "automatisierte Tests in Delivery" },
  },
  {
    value: "34",
    label: { en: "production origins secured", de: "Produktions-Originen abgesichert" },
  },
  { value: "11", label: { en: "public GitHub repositories", de: "oeffentliche GitHub-Repos" } },
  { value: "13", label: { en: "portfolio projects indexed", de: "Portfolio-Projekte indexiert" } },
] satisfies ProfileMetric[];

export const skillGroups = [
  {
    title: { en: "Frontend / Full-Stack", de: "Frontend / Full-Stack" },
    summary: {
      en: "Product UI, client architecture, desktop shells, and regression-safe component systems.",
      de: "Produkt-UI, Client-Architektur, Desktop-Shells und regressionssichere Komponenten-Systeme.",
    },
    accent: "teal",
    items: [
      "TypeScript",
      "JavaScript",
      "Vue 3",
      "Nuxt 3/4",
      "React",
      "Pinia",
      "Vuetify",
      "Tailwind CSS",
      "Electron",
      "Capacitor",
      "Storybook",
      "Playwright",
      "Vitest",
      "FullCalendar",
      "WebView2",
    ],
  },
  {
    title: { en: "ML / Autonomous Driving", de: "ML / Autonomes Fahren" },
    summary: {
      en: "Udacity-era computer vision, neural network, planning, and control projects.",
      de: "Computer Vision, Neural Networks, Planung und Control aus der Udacity-Zeit.",
    },
    accent: "teal",
    items: [
      "TensorFlow",
      "Keras",
      "OpenCV",
      "NumPy",
      "Pandas",
      "Jupyter Notebook",
      "Computer Vision",
      "Neural Networks",
      "CNNs",
      "Lane Detection",
      "Behavioral Cloning",
      "Path Planning",
      "PID Control",
      "ROS",
    ],
  },
  {
    title: { en: "Backend / Platform", de: "Backend / Plattform" },
    summary: {
      en: "API boundaries, secure access control, search, delivery, and operational reliability.",
      de: "API-Grenzen, sichere Zugriffskontrolle, Suche, Delivery und operative Zuverlaessigkeit.",
    },
    accent: "violet",
    items: [
      "Node.js",
      "Python",
      "Directus",
      "REST APIs",
      "WebSocket",
      "Authentication",
      "Authorization",
      "RBAC",
      "OTP",
      "Security Hardening",
      "PostgreSQL",
      "Meilisearch",
      "OpenSearch",
      "Nginx",
      "GitHub Actions",
      "Docker",
      "Kubernetes",
      "ArgoCD",
    ],
  },
  {
    title: { en: "C++ / Native / Systems", de: "C++ / Native / Systeme" },
    summary: {
      en: "Cross-platform clients, native applications, sockets, embedded Linux, and desktop UI.",
      de: "Cross-Platform Clients, native Anwendungen, Sockets, Embedded Linux und Desktop-UI.",
    },
    accent: "amber",
    items: [
      "C++11/14/17/20",
      "C++23",
      "C",
      "C#",
      "Swift",
      "Objective-C",
      "Qt/QML",
      "SQL",
      "Embedded Linux",
      "Windows",
      "macOS",
      "Linux",
      "Cross-Platform Development",
      "Desktop Applications",
    ],
  },
  {
    title: { en: "Engineering Leadership", de: "Engineering Leadership" },
    summary: {
      en: "Architecture, delivery ownership, mentoring, review culture, and stakeholder translation.",
      de: "Architektur, Delivery Ownership, Mentoring, Review-Kultur und Stakeholder-Uebersetzung.",
    },
    accent: "green",
    items: [
      "Software Architecture",
      "System Design",
      "Technical Leadership",
      "Delivery Ownership",
      "Code Review",
      "Mentoring",
      "Hiring",
      "Technical Assessments",
      "Release Management",
      "Incident Prevention",
      "Stakeholder Collaboration",
      "Domain-Driven Design",
    ],
  },
  {
    title: { en: "Quality / Delivery", de: "Qualitaet / Delivery" },
    summary: {
      en: "Risk-based automation, production guardrails, and maintainable release paths.",
      de: "Risikobasierte Automatisierung, Produktionsleitplanken und wartbare Release-Pfade.",
    },
    accent: "coral",
    items: [
      "Jest",
      "Testing Library",
      "Supertest",
      "Prisma Migrations",
      "Structured Logging",
      "OpenTelemetry",
      "Jaeger",
      "Helmet",
      "CORS",
      "CSRF",
      "Rate Limiting",
      "Chromatic",
    ],
  },
] satisfies SkillGroup[];

const udacityEducationCategory = {
  en: "Projects during Udacity education",
  de: "Projekte waehrend der Udacity-Ausbildung",
};

export const localProjects = [
  {
    slug: "small-image-library",
    name: "Small Image Library",
    origin: "local",
    category: { en: "Search platform", de: "Suchplattform" },
    status: { en: "Dockerized coding challenge", de: "Dockerisierte Coding Challenge" },
    summary: {
      en:
        "A media metadata search demo that intentionally uses a service-oriented architecture to show backend, database, cache, frontend, Docker, and testing skill in one bounded system.",
      de:
        "Eine Demo fuer Metadaten-Suche mit serviceorientierter Architektur, die Backend, Datenbank, Cache, Frontend, Docker und Tests in einem begrenzten System zeigt.",
    },
    evidence: {
      en: [
        "NestJS REST API with DTO validation, Helmet, strict CORS, rate limiting, and structured errors.",
        "PostgreSQL media store with Prisma schema, migration, and idempotent seed data.",
        "In-memory inverted index with weighted scoring, prefix matching, filters, sorting, pagination, analytics, and Redis cache.",
        "Next.js frontend with search highlighting and test coverage for core UI behavior.",
      ],
      de: [
        "NestJS REST API mit DTO-Validierung, Helmet, strengem CORS, Rate Limiting und strukturierten Fehlern.",
        "PostgreSQL Media Store mit Prisma Schema, Migration und idempotenten Seed-Daten.",
        "In-Memory Inverted Index mit gewichteter Bewertung, Prefix Matching, Filtern, Sortierung, Pagination, Analytics und Redis Cache.",
        "Next.js Frontend mit Search Highlighting und Testabdeckung fuer zentrales UI-Verhalten.",
      ],
    },
    technologies: [
      "TypeScript",
      "Next.js",
      "React",
      "NestJS",
      "PostgreSQL",
      "Prisma",
      "Redis",
      "Docker",
      "Vitest",
      "Jest",
      "Tailwind CSS",
    ],
    domains: ["Search", "API Design", "Security", "Database", "Testing"],
    links: [
      {
        label: "Local source",
        href: "https://github.com/EugeneTolstikhin/eugenetolstikhin.github.io/tree/main/Projects/SmallImageLibrary",
      },
    ],
  },
  {
    slug: "small-car-auction",
    name: "Small Car Auction",
    origin: "local",
    category: { en: "Full-stack platform", de: "Full-Stack Plattform" },
    status: { en: "Dockerized product slice", de: "Dockerisierter Produkt-Slice" },
    summary: {
      en:
        "A full-stack auction system with authentication, bid workflows, live notifications, observability, and persistence boundaries.",
      de:
        "Ein Full-Stack Auktionssystem mit Authentifizierung, Gebots-Workflows, Live-Benachrichtigungen, Observability und Persistenzgrenzen.",
    },
    evidence: {
      en: [
        "NestJS backend with auth, roles, OTP challenges, CSRF guard, rate limiting, sessions, and Swagger docs.",
        "Prisma/PostgreSQL schema for users, vehicles, auctions, bids, winners, and state transitions.",
        "BullMQ, Redis, Mailpit, and Jaeger are wired into Docker Compose for realistic service dependencies.",
        "Next.js frontend includes card state machines, auth panels, API integration, and Vitest coverage.",
      ],
      de: [
        "NestJS Backend mit Auth, Rollen, OTP Challenges, CSRF Guard, Rate Limiting, Sessions und Swagger Docs.",
        "Prisma/PostgreSQL Schema fuer Nutzer, Fahrzeuge, Auktionen, Gebote, Gewinner und Zustandswechsel.",
        "BullMQ, Redis, Mailpit und Jaeger sind per Docker Compose als realistische Service-Abhaengigkeiten verbunden.",
        "Next.js Frontend mit Card State Machines, Auth Panels, API Integration und Vitest-Abdeckung.",
      ],
    },
    technologies: [
      "TypeScript",
      "Next.js",
      "React",
      "NestJS",
      "PostgreSQL",
      "Prisma",
      "Redis",
      "BullMQ",
      "Server-Sent Events",
      "OpenTelemetry",
      "Jaeger",
      "Docker",
      "Vitest",
      "Jest",
    ],
    domains: ["Auctions", "Auth", "Real-time", "Observability", "Database"],
    links: [
      {
        label: "Local source",
        href: "https://github.com/EugeneTolstikhin/eugenetolstikhin.github.io/tree/main/Projects/SmallCarAuction",
      },
    ],
  },
  {
    slug: "bowling-score",
    name: "Bowling Score",
    origin: "local",
    category: { en: "C++ systems", de: "C++ Systeme" },
    status: { en: "Maintained C++23 version", de: "Gepflegte C++23 Version" },
    summary: {
      en:
        "A bowling hall score application centered on SOLID boundaries, MVC, factories, observer-style updates, sockets, terminal views, and optional Qt/QML UI.",
      de:
        "Eine Bowling-Score-Anwendung mit Fokus auf SOLID-Grenzen, MVC, Factories, Observer-artige Updates, Sockets, Terminal Views und optionale Qt/QML UI.",
    },
    evidence: {
      en: [
        "C++23 application with explicit interfaces for views, logging, config, game, lane, player, frame, and points listeners.",
        "Docker profiles cover plain text, ncurses TUI, and Qt/QML modes for different runtime environments.",
        "Socket-based integration connects the main score app, points generator, and admin panel.",
        "CTest executable validates scoring rules and keeps the rules engine independently testable.",
      ],
      de: [
        "C++23 Anwendung mit expliziten Interfaces fuer Views, Logging, Config, Game, Lane, Player, Frame und Points Listener.",
        "Docker-Profile decken Plain Text, ncurses TUI und Qt/QML Modi fuer verschiedene Laufzeitumgebungen ab.",
        "Socket-basierte Integration verbindet Score App, Points Generator und Admin Panel.",
        "CTest validiert Scoring-Regeln und haelt die Rules Engine unabhaengig testbar.",
      ],
    },
    technologies: [
      "C++23",
      "C",
      "CMake",
      "Qt/QML",
      "ncurses",
      "Sockets",
      "Docker",
      "CTest",
      "Linux",
    ],
    domains: ["Systems", "Desktop UI", "Networking", "Design Patterns", "Testing"],
    links: [
      {
        label: "Local source",
        href: "https://github.com/EugeneTolstikhin/eugenetolstikhin.github.io/tree/main/Projects/BowlingScore",
      },
    ],
  },
  {
    slug: "points-generator",
    name: "Points Generator",
    origin: "local",
    category: { en: "C service", de: "C Service" },
    status: { en: "Socket service", de: "Socket Service" },
    summary: {
      en:
        "A small C service that generates bowling points and feeds the score application through Docker-networked sockets.",
      de:
        "Ein kleiner C Service, der Bowling-Punkte generiert und die Score-Anwendung ueber Docker-vernetzte Sockets versorgt.",
    },
    evidence: {
      en: [
        "CMake-managed C executable with Docker runtime configuration.",
        "Runs as an independent service in the multi-container bowling workflow.",
        "Exposes the points source boundary separately from scoring and admin control.",
      ],
      de: [
        "CMake-verwaltetes C Executable mit Docker Runtime-Konfiguration.",
        "Laeuft als unabhaengiger Service im Multi-Container Bowling-Workflow.",
        "Trennt die Points-Source-Grenze von Scoring und Admin Control.",
      ],
    },
    technologies: ["C", "CMake", "Sockets", "Docker", "Linux"],
    domains: ["Systems", "Networking", "Service Boundary"],
    links: [
      {
        label: "Local source",
        href: "https://github.com/EugeneTolstikhin/eugenetolstikhin.github.io/tree/main/Projects/PointsGenerator",
      },
    ],
  },
  {
    slug: "admin-panel",
    name: "Admin Panel",
    origin: "local",
    category: { en: "Native client", de: "Nativer Client" },
    status: { en: "CLI and Qt client", de: "CLI und Qt Client" },
    summary: {
      en:
        "A companion client for the bowling system, with a C terminal implementation and optional Qt/QML desktop interface.",
      de:
        "Ein Companion Client fuer das Bowling-System mit C Terminal-Implementierung und optionaler Qt/QML Desktop-Oberflaeche.",
    },
    evidence: {
      en: [
        "C command-line client sends player setup data to the bowling score service.",
        "Optional Qt6/QML client demonstrates native UI and network integration.",
        "Docker profiles support terminal and VNC-delivered Qt workflows.",
      ],
      de: [
        "C Kommandozeilen-Client sendet Player-Setup-Daten an den Bowling Score Service.",
        "Optionaler Qt6/QML Client demonstriert native UI und Netzwerkintegration.",
        "Docker-Profile unterstuetzen Terminal- und VNC-basierte Qt Workflows.",
      ],
    },
    technologies: ["C", "C++17", "Qt/QML", "Qt Network", "CMake", "Docker", "VNC"],
    domains: ["Desktop UI", "Networking", "Systems"],
    links: [
      {
        label: "Local source",
        href: "https://github.com/EugeneTolstikhin/eugenetolstikhin.github.io/tree/main/Projects/AdminPanel",
      },
    ],
  },
  {
    slug: "udacity-carnd-capstone",
    name: "Udacity CarND Capstone",
    origin: "education",
    category: udacityEducationCategory,
    status: {
      en: "Self-Driving Car Nanodegree capstone",
      de: "Self-Driving-Car-Nanodegree Capstone",
    },
    summary: {
      en:
        "Final autonomous driving project from Udacity education, connecting perception, planning, and vehicle-control boundaries into one capstone system.",
      de:
        "Abschlussprojekt aus der Udacity-Ausbildung zum autonomen Fahren, das Perception, Planning und Fahrzeugsteuerung in einem Capstone-System verbindet.",
    },
    evidence: {
      en: [
        "Integrates traffic-light handling, waypoint updates, and vehicle-control logic for a simulated autonomous driving flow.",
        "Uses ROS-style service boundaries to keep perception, planning, and control responsibilities separated.",
        "Shows cross-language engineering across CMake, C++, and Python in a robotics-oriented project.",
      ],
      de: [
        "Integriert Ampelverarbeitung, Waypoint-Updates und Fahrzeugsteuerung fuer einen simulierten autonomen Fahrablauf.",
        "Nutzt ROS-aehnliche Service-Grenzen, um Perception, Planning und Control getrennt zu halten.",
        "Zeigt Cross-Language Engineering mit CMake, C++ und Python in einem Robotik-orientierten Projekt.",
      ],
    },
    technologies: ["CMake", "C++", "Python", "ROS", "OpenCV"],
    domains: ["Autonomous Driving", "Robotics", "Perception", "Planning", "Control"],
    links: [
      {
        label: "GitHub source",
        href: "https://github.com/EugeneTolstikhin/udacity-carnd-capstone",
      },
    ],
  },
  {
    slug: "udacity-behaviour-cloning",
    name: "Udacity Behaviour Cloning",
    origin: "education",
    category: udacityEducationCategory,
    status: {
      en: "Neural steering model project",
      de: "Neuronales Steering-Modell Projekt",
    },
    summary: {
      en:
        "Self-driving behavior cloning project that trains a neural network to predict steering behavior from driving-image data.",
      de:
        "Self-Driving Behaviour-Cloning Projekt, das ein neuronales Netz fuer Steering-Vorhersagen aus Fahrbilddaten trainiert.",
    },
    evidence: {
      en: [
        "Frames steering as a supervised learning problem using camera images and driving labels.",
        "Applies image preprocessing and augmentation patterns common in autonomous-driving datasets.",
        "Documents an education project that connects model training decisions with simulator behavior.",
      ],
      de: [
        "Modelliert Steering als Supervised-Learning Problem mit Kamerabildern und Fahr-Labels.",
        "Nutzt Bild-Preprocessing und Augmentation-Patterns fuer autonome Fahrdaten.",
        "Dokumentiert ein Bildungsprojekt, das Trainingsentscheidungen mit Simulator-Verhalten verbindet.",
      ],
    },
    technologies: ["Python", "TensorFlow", "Keras", "OpenCV", "NumPy"],
    domains: ["Autonomous Driving", "Machine Learning", "Computer Vision", "Neural Networks"],
    links: [
      {
        label: "GitHub source",
        href: "https://github.com/EugeneTolstikhin/udacity-behaviour-cloninig",
      },
    ],
  },
  {
    slug: "udacity-path-planning",
    name: "Udacity Path Planning",
    origin: "education",
    category: udacityEducationCategory,
    status: {
      en: "Highway trajectory planning project",
      de: "Highway-Trajectory-Planning Projekt",
    },
    summary: {
      en:
        "Self-driving path planning project focused on safe, smooth trajectory decisions in a highway simulator.",
      de:
        "Self-Driving Path-Planning Projekt mit Fokus auf sichere und gleichmaessige Trajectory-Entscheidungen in einem Highway-Simulator.",
    },
    evidence: {
      en: [
        "Models vehicle behavior decisions such as lane keeping, lane changes, and speed adaptation.",
        "Uses C++ implementation boundaries for simulator integration and trajectory generation.",
        "Captures the planning side of autonomous driving separately from perception and low-level control.",
      ],
      de: [
        "Modelliert Fahrzeugentscheidungen wie Lane Keeping, Lane Changes und Speed Adaptation.",
        "Nutzt C++ Implementierungsgrenzen fuer Simulator-Integration und Trajectory Generation.",
        "Deckt die Planning-Seite des autonomen Fahrens getrennt von Perception und Low-Level Control ab.",
      ],
    },
    technologies: ["C++", "CMake", "Path Planning"],
    domains: ["Autonomous Driving", "Planning", "Simulation", "Algorithms"],
    links: [
      {
        label: "GitHub source",
        href: "https://github.com/EugeneTolstikhin/udacity-path-planning",
      },
    ],
  },
  {
    slug: "udacity-pid-control",
    name: "Udacity PID Control",
    origin: "education",
    category: udacityEducationCategory,
    status: {
      en: "Vehicle control project",
      de: "Vehicle-Control Projekt",
    },
    summary: {
      en:
        "Self-driving control project that applies PID tuning to keep a simulated vehicle stable on track.",
      de:
        "Self-Driving Control Projekt, das PID-Tuning nutzt, um ein simuliertes Fahrzeug stabil auf der Strecke zu halten.",
    },
    evidence: {
      en: [
        "Implements proportional, integral, and derivative feedback behavior for steering correction.",
        "Uses simulator feedback to tune control parameters and reason about overshoot and stability.",
        "Represents the low-level control layer of the Udacity autonomous-driving project sequence.",
      ],
      de: [
        "Implementiert proportionalen, integralen und derivativen Feedback-Anteil fuer Steering Correction.",
        "Nutzt Simulator-Feedback fuer Parameter-Tuning und Analyse von Overshoot und Stabilitaet.",
        "Repraesentiert die Low-Level-Control Ebene der Udacity-Projektserie zum autonomen Fahren.",
      ],
    },
    technologies: ["C++", "CMake", "PID Control"],
    domains: ["Autonomous Driving", "Control", "Simulation", "Algorithms"],
    links: [
      {
        label: "GitHub source",
        href: "https://github.com/EugeneTolstikhin/udacity-pid-control",
      },
    ],
  },
  {
    slug: "udacity-traffic-sign-classifier",
    name: "Udacity Traffic Sign Classifier",
    origin: "education",
    category: udacityEducationCategory,
    status: {
      en: "Computer vision classifier project",
      de: "Computer-Vision-Classifier Projekt",
    },
    summary: {
      en:
        "Traffic-sign classification project from Udacity education, centered on image datasets, CNN training, and model evaluation.",
      de:
        "Traffic-Sign-Classification Projekt aus der Udacity-Ausbildung mit Fokus auf Bilddaten, CNN Training und Modellevaluation.",
    },
    evidence: {
      en: [
        "Builds a supervised classification workflow for road-sign image recognition.",
        "Connects dataset preparation, neural-network architecture, training, and validation results.",
        "Shows applied computer-vision learning in an autonomous-driving context.",
      ],
      de: [
        "Baut einen Supervised-Classification Workflow fuer Road-Sign Image Recognition.",
        "Verbindet Dataset Preparation, Neural-Network Architecture, Training und Validation Results.",
        "Zeigt angewandtes Computer-Vision Lernen im Kontext autonomes Fahren.",
      ],
    },
    technologies: ["Python", "TensorFlow", "Jupyter Notebook", "CNNs", "Computer Vision"],
    domains: ["Machine Learning", "Computer Vision", "Autonomous Driving", "Classification"],
    links: [
      {
        label: "GitHub source",
        href: "https://github.com/EugeneTolstikhin/udacity-traffic-sign-classifier-project",
      },
    ],
  },
  {
    slug: "udacity-tensorflow-lab",
    name: "Udacity TensorFlow Lab",
    origin: "education",
    category: udacityEducationCategory,
    status: {
      en: "TensorFlow learning lab",
      de: "TensorFlow Learning Lab",
    },
    summary: {
      en:
        "Udacity TensorFlow lab that captures early neural-network practice and machine-learning workflow fundamentals.",
      de:
        "Udacity TensorFlow Lab mit frueher Neural-Network Praxis und Grundlagen von Machine-Learning Workflows.",
    },
    evidence: {
      en: [
        "Exercises TensorFlow primitives and model-training workflow in notebook form.",
        "Records educational progression from data preparation to model feedback.",
        "Provides context for later Udacity projects that apply neural networks to driving tasks.",
      ],
      de: [
        "Uebt TensorFlow Primitives und Model-Training Workflow in Notebook-Form.",
        "Dokumentiert den Lernfortschritt von Data Preparation bis Model Feedback.",
        "Gibt Kontext fuer spaetere Udacity-Projekte, die Neural Networks auf Driving Tasks anwenden.",
      ],
    },
    technologies: ["Python", "TensorFlow", "Jupyter Notebook", "Machine Learning"],
    domains: ["Machine Learning", "Neural Networks", "Education"],
    links: [
      {
        label: "GitHub source",
        href: "https://github.com/EugeneTolstikhin/udacity-tensorflow-lab",
      },
    ],
  },
  {
    slug: "udacity-advanced-lane-lines",
    name: "Udacity Advanced Lane Lines",
    origin: "education",
    category: udacityEducationCategory,
    status: {
      en: "Advanced lane detection project",
      de: "Advanced-Lane-Detection Projekt",
    },
    summary: {
      en:
        "Advanced lane-line finding project that applies camera calibration, perspective transforms, and image thresholding.",
      de:
        "Advanced-Lane-Line-Finding Projekt mit Camera Calibration, Perspective Transforms und Image Thresholding.",
    },
    evidence: {
      en: [
        "Uses camera calibration and perspective transforms to improve lane geometry reasoning.",
        "Combines color and gradient thresholding to isolate lane-line candidates.",
        "Extends basic lane detection toward a more robust autonomous-driving perception pipeline.",
      ],
      de: [
        "Nutzt Camera Calibration und Perspective Transforms fuer bessere Lane-Geometry Analyse.",
        "Kombiniert Color- und Gradient-Thresholding, um Lane-Line Candidates zu isolieren.",
        "Erweitert Basic Lane Detection zu einer robusteren Perception Pipeline fuer autonomes Fahren.",
      ],
    },
    technologies: ["Python", "OpenCV", "NumPy", "Jupyter Notebook", "Computer Vision"],
    domains: ["Computer Vision", "Autonomous Driving", "Perception", "Image Processing"],
    links: [
      {
        label: "GitHub source",
        href: "https://github.com/EugeneTolstikhin/udacity-advanced-lane-lines-finding",
      },
    ],
  },
  {
    slug: "udacity-find-lane-lines",
    name: "Udacity Find Lane Lines",
    origin: "education",
    category: udacityEducationCategory,
    status: {
      en: "Intro lane detection project",
      de: "Intro-Lane-Detection Projekt",
    },
    summary: {
      en:
        "Introductory lane-line finding project from Udacity education, focused on image-processing fundamentals for road scenes.",
      de:
        "Einstiegsprojekt zur Lane-Line Detection aus der Udacity-Ausbildung mit Fokus auf Image-Processing Grundlagen fuer Strassenszenen.",
    },
    evidence: {
      en: [
        "Applies edge detection and line-detection techniques to identify lane markings.",
        "Uses notebook-based experimentation to make the perception pipeline inspectable.",
        "Establishes the computer-vision baseline that later advanced lane projects build on.",
      ],
      de: [
        "Nutzt Edge Detection und Line Detection, um Lane Markings zu erkennen.",
        "Verwendet Notebook-basierte Experimente, um die Perception Pipeline nachvollziehbar zu machen.",
        "Legt die Computer-Vision Baseline fuer spaetere Advanced-Lane Projekte.",
      ],
    },
    technologies: ["Python", "OpenCV", "NumPy", "Jupyter Notebook", "Computer Vision"],
    domains: ["Computer Vision", "Autonomous Driving", "Perception", "Image Processing"],
    links: [
      {
        label: "GitHub source",
        href: "https://github.com/EugeneTolstikhin/udacity-find-lane-lines",
      },
    ],
  },
] satisfies PortfolioProject[];

export const githubRepos = [
  {
    name: "eugenetolstikhin.github.io",
    description: {
      en: "The web project where personal projects and software architecture knowledge are collected.",
      de: "Das Webprojekt fuer persoenliche Projekte und Softwarearchitektur-Wissen.",
    },
    url: "https://github.com/EugeneTolstikhin/eugenetolstikhin.github.io",
    language: "TypeScript",
    topics: [],
    stars: 0,
    forks: 0,
    updatedAt: "2026-05-04",
    pushedAt: "2026-05-04",
  },
  {
    name: "SmallImageLibraryPureNext",
    description: {
      en: "Pure Next.js variant of the image library search challenge.",
      de: "Pure Next.js Variante der Image-Library Search Challenge.",
    },
    url: "https://github.com/EugeneTolstikhin/SmallImageLibraryPureNext",
    language: "TypeScript",
    topics: [],
    stars: 0,
    forks: 0,
    updatedAt: "2026-05-04",
    pushedAt: "2026-05-04",
  },
  {
    name: "Posix",
    description: { en: "C/POSIX practice repository.", de: "C/POSIX Practice Repository." },
    url: "https://github.com/EugeneTolstikhin/Posix",
    language: "C",
    topics: [],
    stars: 0,
    forks: 0,
    updatedAt: "2021-10-04",
    pushedAt: "2021-10-04",
  },
  {
    name: "udacity-carnd-capstone",
    description: {
      en: "Final project for a self-driving car nanodegree.",
      de: "Abschlussprojekt fuer ein Self-Driving-Car Nanodegree.",
    },
    url: "https://github.com/EugeneTolstikhin/udacity-carnd-capstone",
    language: "CMake",
    topics: [],
    stars: 0,
    forks: 0,
    updatedAt: "2021-03-17",
    pushedAt: "2021-03-17",
  },
  {
    name: "udacity-behaviour-cloninig",
    description: {
      en: "Self-driving behavior cloning project.",
      de: "Self-Driving Behaviour-Cloning Projekt.",
    },
    url: "https://github.com/EugeneTolstikhin/udacity-behaviour-cloninig",
    language: "Python",
    topics: [],
    stars: 0,
    forks: 0,
    updatedAt: "2021-02-22",
    pushedAt: "2021-02-22",
  },
  {
    name: "udacity-path-planning",
    description: { en: "Self-driving path planning project.", de: "Self-Driving Path-Planning Projekt." },
    url: "https://github.com/EugeneTolstikhin/udacity-path-planning",
    language: "C++",
    topics: [],
    stars: 0,
    forks: 0,
    updatedAt: "2021-02-15",
    pushedAt: "2021-02-15",
  },
  {
    name: "udacity-pid-control",
    description: { en: "Self-driving PID control project.", de: "Self-Driving PID-Control Projekt." },
    url: "https://github.com/EugeneTolstikhin/udacity-pid-control",
    language: "C++",
    topics: [],
    stars: 0,
    forks: 0,
    updatedAt: "2021-02-07",
    pushedAt: "2021-02-07",
  },
  {
    name: "udacity-traffic-sign-classifier-project",
    description: {
      en: "Traffic sign classifier project.",
      de: "Traffic-Sign-Classifier Projekt.",
    },
    url: "https://github.com/EugeneTolstikhin/udacity-traffic-sign-classifier-project",
    language: "HTML",
    topics: [],
    stars: 0,
    forks: 0,
    updatedAt: "2021-01-22",
    pushedAt: "2021-01-22",
  },
  {
    name: "udacity-tensorflow-lab",
    description: { en: "TensorFlow lab.", de: "TensorFlow Lab." },
    url: "https://github.com/EugeneTolstikhin/udacity-tensorflow-lab",
    language: "Jupyter Notebook",
    topics: [],
    stars: 0,
    forks: 0,
    updatedAt: "2020-12-07",
    pushedAt: "2020-12-07",
  },
  {
    name: "udacity-advanced-lane-lines-finding",
    description: {
      en: "Advanced lane line finding project from the self-driving track.",
      de: "Advanced-Lane-Line-Finding Projekt aus dem Self-Driving Track.",
    },
    url: "https://github.com/EugeneTolstikhin/udacity-advanced-lane-lines-finding",
    language: "Jupyter Notebook",
    topics: [],
    stars: 0,
    forks: 0,
    updatedAt: "2020-12-04",
    pushedAt: "2020-12-04",
  },
  {
    name: "udacity-find-lane-lines",
    description: {
      en: "Lane line finding project from the self-driving track.",
      de: "Lane-Line-Finding Projekt aus dem Self-Driving Track.",
    },
    url: "https://github.com/EugeneTolstikhin/udacity-find-lane-lines",
    language: "Jupyter Notebook",
    topics: [],
    stars: 0,
    forks: 0,
    updatedAt: "2020-11-23",
    pushedAt: "2020-11-23",
  },
] satisfies GitHubRepo[];

export const principles = [
  {
    title: { en: "Evidence over slogans", de: "Evidenz statt Schlagworte" },
    body: {
      en: "Every skill category links back to local projects or public repository history.",
      de: "Jede Skill-Kategorie verweist auf lokale Projekte oder oeffentliche Repository-Historie.",
    },
  },
  {
    title: { en: "Static by design", de: "Statisch per Design" },
    body: {
      en: "The site is generated ahead of time for GitHub Pages, with no runtime API, database, or secret dependency.",
      de: "Die Seite wird fuer GitHub Pages vorab generiert, ohne Runtime-API, Datenbank oder Secret-Abhaengigkeit.",
    },
  },
  {
    title: { en: "Library", de: "Bibliothek" },
    body: {
      en: "The primary interface is the searchable project and skill library.",
      de: "Die primaere Oberflaeche ist die durchsuchbare Projekt- und Skill-Bibliothek.",
    },
  },
] satisfies Principle[];

export const spokenLanguages = [
  { language: {en: "Ukrainian", de: "Ukrainisch"}, level: {en: "Native", de: "Muttersprache"} },
  { language: {en: "Russian", de: "Russisch"}, level: {en: "Native", de: "Muttersprache"} },
  { language: {en: "English", de: "Englisch"}, level: {en: "C1", de: "C1"} },
  { language: {en: "German", de: "Deutsch"}, level: {en: "B2+", de: "B2+" } }
] satisfies { language: { en: string; de: string }; level: { en: string; de: string } }[];

export const githubSnapshot = {
  source: "https://api.github.com/users/EugeneTolstikhin/repos?per_page=100&sort=updated",
  syncedAt: "2026-05-12",
};
