// === STATE & CONFIG ===
const state = {
  currentTab: 'raw',
  requests: [],
  responses: [],
  logs: [],
  serverHost: localStorage.getItem('labServerHost') || 'localhost',
  serverPort: localStorage.getItem('labServerPort') || '2000',
  totalRequests: 0,
  failedRequests: 0,
  totalTime: 0,
};

// Initialize nav config
document.addEventListener('DOMContentLoaded', () => {
  document.getElementById('navServerHost').value = state.serverHost;
  document.getElementById('navServerPort').value = state.serverPort;
  document.getElementById('dashServerHost').value = state.serverHost;
  document.getElementById('dashServerPort').value = state.serverPort;
  document.getElementById('displayServerHost').textContent = state.serverHost;
  document.getElementById('displayServerPort').textContent = state.serverPort;
  updateDashboard();
  loadTemplate();
  setInterval(updateDashboard, 5000);
});

function updateServerConfig() {
  const host = document.getElementById('dashServerHost').value.trim() || 'localhost';
  const port = document.getElementById('dashServerPort').value.trim() || '2000';

  state.serverHost = host;
  state.serverPort = port;

  localStorage.setItem('labServerHost', host);
  localStorage.setItem('labServerPort', port);

  // Update all displays
  document.getElementById('navServerHost').value = host;
  document.getElementById('navServerPort').value = port;
  document.getElementById('displayServerHost').textContent = host;
  document.getElementById('displayServerPort').textContent = port;

  loadTemplate();
  showToast(`✓ Target updated: ${host}:${port}`);
}

function updateServerConfigFromNav() {
  const host = document.getElementById('navServerHost').value.trim() || 'localhost';
  const port = document.getElementById('navServerPort').value.trim() || '2000';

  state.serverHost = host;
  state.serverPort = port;

  localStorage.setItem('labServerHost', host);
  localStorage.setItem('labServerPort', port);

  // Update all references
  document.getElementById('serverPort').textContent = port;
  updateDashboard();

  // Show toast
  showToast(`✓ Serveur: ${host}:${port}`);
}

function showToast(message) {
  const toast = document.createElement('div');
  toast.style.cssText = `
    position: fixed;
    bottom: 2rem;
    right: 2rem;
    background: rgba(34, 197, 94, 0.9);
    color: white;
    padding: 0.75rem 1.5rem;
    border-radius: 8px;
    z-index: 1000;
    animation: slideIn 0.3s ease;
  `;
  toast.textContent = message;
  document.body.appendChild(toast);

  setTimeout(() => {
    toast.style.animation = 'slideOut 0.3s ease';
    setTimeout(() => toast.remove(), 300);
  }, 2500);
}

// Add toast animations to styles if not present
if (!document.querySelector('style[data-toast]')) {
  const style = document.createElement('style');
  style.setAttribute('data-toast', '');
  style.textContent = `
    @keyframes slideIn {
      from { transform: translateX(400px); opacity: 0; }
      to { transform: translateX(0); opacity: 1; }
    }
    @keyframes slideOut {
      from { transform: translateX(0); opacity: 1; }
      to { transform: translateX(400px); opacity: 0; }
    }
  `;
  document.head.appendChild(style);
}

function escapeHtml(value) {
  return value
    .replace(/&/g, '&amp;')
    .replace(/</g, '&lt;')
    .replace(/>/g, '&gt;')
    .replace(/"/g, '&quot;')
    .replace(/'/g, '&#39;');
}

// === NAVIGATION ===
document.querySelectorAll('.nav-link').forEach((btn) => {
  btn.addEventListener('click', () => {
    const section = btn.getAttribute('data-section');
    switchSection(section);
  });
});

function switchSection(sectionName) {
  // Hide all views
  document.querySelectorAll('.view').forEach((view) => {
    view.classList.remove('active');
  });

  // Show target view
  const targetView = document.getElementById(sectionName);
  if (targetView) {
    targetView.classList.add('active');
  }

  // Update navbar
  document.querySelectorAll('.nav-link').forEach((btn) => {
    btn.classList.remove('active');
  });
  document.querySelector(`[data-section="${sectionName}"]`).classList.add('active');
}

// === CURSOR FOLLOW ===
const cursorFollow = document.getElementById('cursorFollow');
document.addEventListener('mousemove', (e) => {
  if (window.matchMedia('(max-width: 768px)').matches) return;
  cursorFollow.style.opacity = '1';
  cursorFollow.style.left = `${e.clientX - 100}px`;
  cursorFollow.style.top = `${e.clientY - 100}px`;
});

document.addEventListener('mouseout', () => {
  cursorFollow.style.opacity = '0';
});

// === DASHBOARD ===
function updateDashboard() {
  const uptime = Math.floor(Math.random() * 1000);
  document.getElementById('serverUptime').textContent = `${uptime}s`;
  document.getElementById('totalRequests').textContent = state.totalRequests;

  const avgLatency = state.totalRequests > 0 ? Math.round(state.totalTime / state.totalRequests) : 0;
  document.getElementById('avgLatency').textContent = avgLatency;

  const rps = state.totalRequests > 0 ? Math.min(state.totalRequests / (uptime || 1), 999) : 0;
  document.getElementById('rps').textContent = rps.toFixed(1);

  const errRate = state.totalRequests > 0 ? ((state.failedRequests / state.totalRequests) * 100).toFixed(1) : 0;
  document.getElementById('errRate').textContent = `${errRate}%`;
}

function quickTest() {
  const testReq = `GET / HTTP/1.1\r\nHost: ${state.serverHost}:${state.serverPort}\r\n\r\n`;
  const startTime = performance.now();

  fetch(`http://${state.serverHost}:${state.serverPort}/`, {
    method: 'GET',
    headers: { 'Connection': 'close' },
  })
    .then((response) => response.text().then((body) => ({ status: response.status, body })))
    .then((result) => {
      const time = Math.round(performance.now() - startTime);
      const resultDiv = document.getElementById('quickTestResult');
      resultDiv.classList.add('showing');
      resultDiv.textContent = `Status: ${result.status}\nTime: ${time}ms\n\n${result.body.substring(0, 200)}...`;
      logRequest(testReq, result.status, time);
      updateDashboard();
    })
    .catch((error) => {
      const time = Math.round(performance.now() - startTime);
      const resultDiv = document.getElementById('quickTestResult');
      resultDiv.classList.add('showing');
      resultDiv.textContent = `Error: ${error.message}\nMake sure server is running at ${state.serverHost}:${state.serverPort}`;
    });
}

// === RAW HTTP PLAYGROUND ===
function sendRawRequest() {
  const httpText = document.getElementById('httpEditor').value;
  if (!httpText.trim()) {
    alert('Enter raw HTTP request');
    return;
  }

  const startTime = performance.now();
  parseRawHTTP(httpText);

  // Extract request details
  const lines = httpText.split('\n');
  const firstLine = lines[0].trim().split(' ');
  const method = firstLine[0] || 'GET';
  const path = firstLine[1] || '/';

  // Extract body if present
  let body = null;
  const emptyLineIdx = httpText.indexOf('\n\n');
  if (emptyLineIdx !== -1) {
    body = httpText.substring(emptyLineIdx + 2).trim();
  }

  // Extract headers
  const headers = {};
  lines.slice(1).forEach((line) => {
    if (line.trim() && line.includes(':')) {
      const [key, value] = line.split(':');
      const keyTrim = key.trim();
      if (keyTrim.toLowerCase() !== 'host') {
        headers[keyTrim] = value.trim();
      }
    }
  });

  // Make real fetch request
  const url = `http://${state.serverHost}:${state.serverPort}${path}`;

  fetch(url, {
    method: method,
    headers: headers,
    body: body && method !== 'GET' && method !== 'HEAD' ? body : undefined,
  })
    .then((response) => {
      const time = Math.round(performance.now() - startTime);

      // Collect headers
      const responseHeaders = {};
      response.headers.forEach((value, name) => {
        responseHeaders[name] = value;
      });

      return response.text().then((text) => ({
        status: response.status,
        statusText: response.statusText,
        headers: responseHeaders,
        body: text,
        time: time,
      }));
    })
    .then((response) => {
      const statusCode = response.status;
      document.getElementById('statusCode').textContent = `${statusCode} ${response.statusText}`;
      document.getElementById('responseTime').textContent = response.time;

      // Build raw response display
      let rawResponse = `HTTP/1.1 ${statusCode} ${response.statusText}\r\n`;
      Object.entries(response.headers).forEach(([key, value]) => {
        rawResponse += `${key}: ${value}\r\n`;
      });
      rawResponse += '\r\n' + response.body;

      document.getElementById('responseBody').textContent = rawResponse;

      state.totalRequests++;
      if (![200, 201, 204, 301, 302, 304].includes(parseInt(statusCode))) {
        state.failedRequests++;
      }
      state.totalTime += response.time;

      logRequest(httpText, statusCode, response.time);
      updateDashboard();

      showToast(`✓ Request succeeded: ${statusCode}`);
    })
    .catch((error) => {
      const time = Math.round(performance.now() - startTime);

      document.getElementById('statusCode').textContent = 'ERROR';
      document.getElementById('responseTime').textContent = time;
      document.getElementById('responseBody').textContent = `Error: ${error.message}\n\nMake sure:\n1. Server is running at ${state.serverHost}:${state.serverPort}\n2. Path exists on the server`;

      state.totalRequests++;
      state.failedRequests++;
      state.totalTime += time;

      logRequest(httpText, 'ERROR', time);
      updateDashboard();

      showToast(`✗ Request failed: ${error.message}`);
    });
}

function parseRawHTTP(raw) {
  const lines = raw.split('\n');
  const firstLine = lines[0].trim().split(' ');
  const parserOutput = document.getElementById('parserOutput');
  parserOutput.innerHTML = '';

  const fields = [
    { label: 'Method', value: firstLine[0] || '---' },
    { label: 'Path', value: firstLine[1] || '---' },
    { label: 'Version', value: firstLine[2] || '---' },
  ];

  lines.forEach((line) => {
    if (line.includes(':') && !line.startsWith('HTTP')) {
      const [key, value] = line.split(':');
      fields.push({ label: key.trim(), value: value.trim() });
    }
  });

  fields.forEach((field) => {
    const row = document.createElement('tr');
    row.innerHTML = `<td class="bold">${field.label}</td><td><code>${field.value}</code></td>`;
    parserOutput.appendChild(row);
  });
}

function clearEditor() {
  document.getElementById('httpEditor').value = '';
  document.getElementById('responseBody').textContent = 'Response will appear here...';
}

function loadTemplate() {
  const template = `POST /api/data HTTP/1.1\r\nHost: ${state.serverHost}:${state.serverPort}\r\nContent-Type: application/json\r\nContent-Length: 27\r\n\r\n{"name": "test", "id": 1}`;
  document.getElementById('httpEditor').value = template;
}

function switchTab(tab) {
  state.currentTab = tab;
  document.querySelectorAll('.tab-btn').forEach((btn) => btn.classList.remove('active'));
  event.target.classList.add('active');
}

// === UPLOAD ===
const uploadArea = document.getElementById('uploadArea');
const fileInput = document.getElementById('fileInput');
const uploadQueue = document.getElementById('uploadQueue');

uploadArea.addEventListener('click', () => fileInput.click());

uploadArea.addEventListener('dragover', (e) => {
  e.preventDefault();
  uploadArea.classList.add('drag-over');
});

uploadArea.addEventListener('dragleave', () => {
  uploadArea.classList.remove('drag-over');
});

uploadArea.addEventListener('drop', (e) => {
  e.preventDefault();
  uploadArea.classList.remove('drag-over');
  fileInput.files = e.dataTransfer.files;
  displayUploadQueue();
});

fileInput.addEventListener('change', displayUploadQueue);

function displayUploadQueue() {
  uploadQueue.innerHTML = '';
  const files = fileInput.files;
  if (files.length > 0) {
    document.getElementById('uploadBtn').disabled = false;
  }

  Array.from(files).forEach((file) => {
    const item = document.createElement('div');
    item.className = 'upload-item';
    item.innerHTML = `<span>${file.name} (${(file.size / 1024).toFixed(2)} KB)</span><span>✓</span>`;
    uploadQueue.appendChild(item);
  });

  // Generate request preview
  generateUploadRequestPreview();
}

function generateUploadRequestPreview() {
  const files = fileInput.files;
  if (files.length === 0) {
    document.getElementById('uploadRequestPreview').innerHTML = '<p class="muted">Select files to preview request...</p>';
    return;
  }

  const uploadPath = document.getElementById('uploadPath').value || '/';
  const formName = document.getElementById('uploadFormName').value || 'file';
  const host = state.serverHost;
  const port = state.serverPort;

  // Calculate total size
  const totalSize = Array.from(files).reduce((a, f) => a + f.size, 0);

  // Build boundary for multipart (simulated)
  const boundary = 'WebservBoundary' + Math.random().toString(36).substring(7);

  // Calculate content length
  let contentLength = 0;
  Array.from(files).forEach((file) => {
    contentLength += boundary.length + 4; // --boundary\r\n
    contentLength += `Content-Disposition: form-data; name="${formName}"; filename="${file.name}"\r\n`.length;
    contentLength += 'Content-Type: application/octet-stream\r\n\r\n'.length;
    contentLength += file.size + 2; // \r\n
  });
  contentLength += boundary.length + 6; // --boundary--\r\n

  let preview = `POST ${uploadPath} HTTP/1.1\r\n`;
  preview += `Host: ${host}:${port}\r\n`;
  preview += `Content-Type: multipart/form-data; boundary=${boundary}\r\n`;
  preview += `Content-Length: ${contentLength}\r\n`;
  preview += `Connection: close\r\n`;
  preview += `\r\n`;
  preview += `[Binary multipart body with ${files.length} file(s)]\r\n\r\n`;
  preview += `File Details:\r\n`;

  Array.from(files).forEach((file, idx) => {
    preview += `  ${idx + 1}. ${file.name} (${(file.size / 1024).toFixed(2)} KB)\r\n`;
  });

  preview += `\r\nTotal Size: ${(totalSize / 1024).toFixed(2)} KB`;

  document.getElementById('uploadRequestPreview').textContent = preview;
}

// Listen to upload path/form name changes
document.addEventListener('DOMContentLoaded', () => {
  const uploadPath = document.getElementById('uploadPath');
  const uploadFormName = document.getElementById('uploadFormName');

  // Ensure defaults are set
  if (uploadPath) {
    if (!uploadPath.value || uploadPath.value.trim() === '') {
      uploadPath.value = '/';
    }
    uploadPath.addEventListener('change', generateUploadRequestPreview);
    uploadPath.addEventListener('input', generateUploadRequestPreview);
  }
  if (uploadFormName) {
    if (!uploadFormName.value || uploadFormName.value.trim() === '') {
      uploadFormName.value = 'file';
    }
    uploadFormName.addEventListener('change', generateUploadRequestPreview);
    uploadFormName.addEventListener('input', generateUploadRequestPreview);
  }
});

function switchUploadTab(tab) {
  document.querySelectorAll('.request-tabs .tab-btn').forEach((btn) => btn.classList.remove('active'));
  event.target.classList.add('active');
}

function uploadFiles() {
  const files = fileInput.files;
  if (files.length === 0) return;

  let uploadPath = document.getElementById('uploadPath').value.trim();
  if (!uploadPath || uploadPath === '') uploadPath = '/';
  
  let formName = document.getElementById('uploadFormName').value.trim();
  if (!formName || formName === '') formName = 'file';

  const startTime = performance.now();
  const responseDiv = document.getElementById('uploadResponse');
  const responseFrame = document.getElementById('uploadResponseFrame');
  const totalSize = Array.from(files).reduce((a, f) => a + f.size, 0);
  const uploadUrl = `http://${state.serverHost}:${state.serverPort}${uploadPath}`;
  
  responseDiv.innerHTML = `<div class="log-entry">[UPLOAD] Target: POST ${uploadPath}</div>
    <div class="log-entry">[UPLOAD] Server: ${state.serverHost}:${state.serverPort}</div>
    <div class="log-entry">[UPLOAD] Form Field: "${formName}"</div>
    <div class="log-entry">[UPLOAD] Sending ${files.length} file(s) (${(totalSize / 1024).toFixed(2)} KB)...</div>
    <div class="log-entry">[UPLOAD] The real server response will render in the frame below.</div>`;

  responseFrame.src = 'about:blank';
  responseFrame.onload = () => {
    try {
      const frameDoc = responseFrame.contentDocument || responseFrame.contentWindow.document;
      if (!frameDoc || !frameDoc.body) return;

      const bodyText = frameDoc.body.innerText.trim();
      if (!bodyText) return;

      const time = Math.round(performance.now() - startTime);
      responseDiv.innerHTML += `<div class="log-entry">[201] Server responded</div>
        <div class="log-entry">Response time: ${time}ms</div>
        <div class="log-entry"><strong>Response rendered below:</strong></div>
        <pre class="response-content">${escapeHtml(bodyText)}</pre>`;

      state.totalRequests++;
      state.totalTime += time;
      updateDashboard();
      logRequest(`POST ${uploadPath} (${files.length} files)`, 201, time);
      showToast('✓ Upload completed');
    } catch (error) {
      // Cross-origin frames are expected on some setups; the response is still visible in the frame.
    }
  };

  const form = document.createElement('form');
  form.method = 'POST';
  form.action = uploadUrl;
  form.enctype = 'multipart/form-data';
  form.target = 'uploadResponseFrame';
  form.style.display = 'none';

  const transfer = document.createElement('input');
  transfer.type = 'file';
  transfer.name = formName;
  transfer.multiple = true;

  const dataTransfer = new DataTransfer();
  Array.from(files).forEach((file) => dataTransfer.items.add(file));
  transfer.files = dataTransfer.files;

  form.appendChild(transfer);
  document.body.appendChild(form);

  // Submit through a regular form so the browser can display the actual response body.
  form.submit();
  setTimeout(() => form.remove(), 1000);
}

// === REQUEST LOGGING ===
function logRequest(request, status, time) {
  const timestamp = new Date().toLocaleTimeString();
  const method = request.split(' ')[0];
  const path = request.split(' ')[1] || '/';

  const logEntry = `[${timestamp}] ${method} ${path} → ${status} (${time}ms)`;
  state.logs.unshift(logEntry);

  const logTerminal = document.getElementById('requestLog');
  const entryEl = document.createElement('div');
  entryEl.className = 'log-entry';
  entryEl.textContent = logEntry;

  if (logTerminal.querySelector('.muted')) {
    logTerminal.innerHTML = '';
  }

  logTerminal.insertBefore(entryEl, logTerminal.firstChild);

  // Keep only last 50 entries
  if (state.logs.length > 50) {
    logTerminal.removeChild(logTerminal.lastChild);
    state.logs.pop();
  }
}

function clearLogs() {
  document.getElementById('requestLog').innerHTML = '<p class="muted">Requests will be logged here...</p>';
  state.logs = [];
}

// === INITIALIZATION ===
// Initialization is done in DOMContentLoaded handler above
