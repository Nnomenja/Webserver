// Load server config from localStorage
const serverConfig = {
  host: localStorage.getItem('serverHost') || 'localhost',
  port: localStorage.getItem('serverPort') || '8080',
};

const terminalContent = () =>
  `$ ./server\n\n[INFO] Booting HTTP/1.1 server...\n[OK] Listening on ${serverConfig.host}:${serverConfig.port}\n[OK] Keep-Alive enabled\n[READY] Awaiting connections`;

const typedTerminal = document.getElementById("typedTerminal");
const menuToggle = document.getElementById("menuToggle");
const navLinks = document.getElementById("navLinks");
const cursorGlow = document.getElementById("cursorGlow");
const statValues = document.querySelectorAll(".stat-card__value");

// Save server configuration
function saveServerConfig() {
  const host = document.getElementById('serverHost').value || 'localhost';
  const port = document.getElementById('serverPort').value || '8080';

  localStorage.setItem('serverHost', host);
  localStorage.setItem('serverPort', port);

  serverConfig.host = host;
  serverConfig.port = port;

  // Test connection to server
  const testUrl = `http://${host}:${port}/`;
  fetch(testUrl, { method: 'HEAD' })
    .then(() => {
      // Show success message
      const statusEl = document.getElementById('configStatus');
      statusEl.textContent = `✓ Configuré et connecté: ${host}:${port}`;
      statusEl.classList.add('showing');

      // Refresh terminal
      typeTerminal(terminalContent(), 22);

      setTimeout(() => {
        statusEl.classList.remove('showing');
      }, 3000);
    })
    .catch(() => {
      // Server not reachable, but config saved
      const statusEl = document.getElementById('configStatus');
      statusEl.textContent = `⚠ Configuré: ${host}:${port} (serveur non joignable - vérifiez qu'il est lancé)`;
      statusEl.style.color = '#f59e0b';
      statusEl.classList.add('showing');

      // Still refresh terminal with new config
      typeTerminal(terminalContent(), 22);

      setTimeout(() => {
        statusEl.classList.remove('showing');
        statusEl.style.color = '#22c55e';
      }, 4000);
    });
}

function typeTerminal(text, delay) {
  let index = 0;

  function write() {
    if (!typedTerminal) {
      return;
    }
    typedTerminal.textContent = text.slice(0, index);
    index += 1;
    if (index <= text.length) {
      setTimeout(write, delay);
    }
  }

  write();
}

function revealOnScroll() {
  const observer = new IntersectionObserver(
    (entries) => {
      entries.forEach((entry) => {
        if (entry.isIntersecting) {
          entry.target.classList.add("is-visible");
          observer.unobserve(entry.target);
        }
      });
    },
    { threshold: 0.18 }
  );

  document.querySelectorAll(".reveal").forEach((element) => observer.observe(element));
}

function animateCounters() {
  const observer = new IntersectionObserver(
    (entries) => {
      entries.forEach((entry) => {
        if (!entry.isIntersecting) {
          return;
        }

        const element = entry.target;
        const target = Number(element.getAttribute("data-target"));
        const duration = 1300;
        const start = performance.now();

        function frame(now) {
          const progress = Math.min((now - start) / duration, 1);
          const value = target * (1 - Math.pow(1 - progress, 3));
          const hasDecimal = String(target).includes(".");
          element.textContent = hasDecimal ? value.toFixed(2) : Math.round(value).toString();

          if (progress < 1) {
            requestAnimationFrame(frame);
          }
        }

        requestAnimationFrame(frame);
        observer.unobserve(element);
      });
    },
    { threshold: 0.5 }
  );

  statValues.forEach((element) => observer.observe(element));
}

function initMobileMenu() {
  if (!menuToggle || !navLinks) {
    return;
  }

  menuToggle.addEventListener("click", () => {
    const isOpen = navLinks.classList.toggle("is-open");
    menuToggle.setAttribute("aria-expanded", String(isOpen));
  });
}

function initCursorGlow() {
  if (!cursorGlow || window.matchMedia("(max-width: 900px)").matches) {
    return;
  }

  window.addEventListener("mousemove", (event) => {
    cursorGlow.style.opacity = "1";
    cursorGlow.style.left = `${event.clientX}px`;
    cursorGlow.style.top = `${event.clientY}px`;
  });

  window.addEventListener("mouseout", () => {
    cursorGlow.style.opacity = "0";
  });
}

typeTerminal(terminalContent(), 22);
revealOnScroll();
animateCounters();
initMobileMenu();
initCursorGlow();
