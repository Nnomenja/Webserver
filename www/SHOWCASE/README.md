# Webserv - Web Applications & Testing Lab

Ce dossier contient les applications web pour showcaser, tester et déboguer le serveur HTTP/1.1 Webserv.

## 📁 Applications

### 1. **Landing Page** (`homePage/`)
- **URL**: `http://localhost:8080/homePage/`
- **Description**: Page d'accueil premium showcasing le projet Webserv
- **Fonctionnalités**:
  - Terminal en temps réel avec output du serveur
  - Configuration du serveur (Host + Port)
  - Affichage dynamique de la config serveur

**Configuration**:
1. Ouvre `http://localhost:8080/homePage/`
2. Va à la section "Server Configuration"
3. Entre Host (ex: `localhost`, `127.0.0.1`, `192.168.1.100`)
4. Entre Port (ex: `8080`, `3000`)
5. Clique "Enregistrer Configuration"
6. Le terminal affiche les paramètres en direct

---

### 2. **HTTP Testing Laboratory** (`testing-lab/`)
- **URL**: `http://localhost:8080/testing-lab/`
- **Description**: Plateforme professionnelle de test HTTP/1.1 pour déboguer le serveur
- **Fonctionnalités complètes**:

#### **Dashboard**
- Configuration du serveur cible (Host + Port)
- Statut du serveur en temps réel
- Métriques HTTP (latence, req/s, taux erreur)
- Test rapide

#### **Raw HTTP Playground**
- Éditeur HTTP avec syntaxe complète
- **Envoie des requêtes RÉELLES au serveur**
- Affiche la réponse brute HTTP
- Visualisation du parser HTTP
- Temps de réponse réel

Exemple d'utilisation:
```http
POST /api/data HTTP/1.1
Host: localhost:8080
Content-Type: application/json
Content-Length: 27

{"name": "test", "id": 1}
```

#### **Upload Testing**
- Upload multipart/form-data **réel**
- Drag & drop files
- Affiche statut HTTP réel du serveur
- Logs détaillés

#### **Redirect Tester**
- Test les chaînes de redirects **réelles**
- Affichage du flux de redirections
- Capture status codes (301, 302, etc.)

#### **Static Files**
- Explore les fichiers statiques du serveur
- Preview de fichiers

#### **CGI Scripts**
- Exécution **réelle** de scripts CGI
- GET/POST parameters
- Affichage output en direct

#### **Request Logs**
- Terminal affichant tous les requêtes
- Format: `[TIME] METHOD PATH → STATUS (XXms)`
- Auto-scroll in real-time

---

## 🚀 Configuration du Serveur Cible

### Dans la Navbar (Testing Lab)
```
[localhost    ] : [8080    ] [✓]
```

### Dans le Dashboard
```
Server Host: localhost
Server Port: 8080
[Update Target]
```

Ces deux méthodes sauvegardent la config en **localStorage** et persiste entre les sessions.

---

## 🌐 Requêtes HTTP Réelles

### ✅ Ce qui fonctionne vraiment:

**Raw HTTP Requests**
- Fetch réel vers le serveur
- Affichage des headers réels
- Body réel de la réponse
- Temps de réponse réel

**File Uploads**
- POST multipart/form-data réel
- Réponse du serveur en direct

**CGI Execution**
- Exécution réelle des scripts
- Output du serveur

**Redirect Testing**
- Requête réelle sans follow automatique
- Capture des status redirects

**Metrics**
- Compteurs et temps réels mesurés
- Basés sur les requêtes réelles

---

## ⚠️ CORS et Localhost

### Si tu test en local (localhost):
✅ Tout fonctionne (même domaine, même port)

### Si tu test cross-origin ou sur IP différente:
⚠️ Tu pourrais avoir des erreurs CORS si le serveur n'envoie pas les headers:
```
Access-Control-Allow-Origin: *
```

**Solution**: Configure ton serveur pour envoyer ces headers CORS, ou test depuis `localhost` directement.

---

## 📝 Exemples de Requêtes

### Test GET Simple
```http
GET / HTTP/1.1
Host: localhost:8080

```

### Upload Fichier
1. Clique "Upload Test"
2. Drag & drop des fichiers
3. Clique "Upload Files"

### Test CGI
```
Script Path: /cgi/script.py
Execute Button
```

### Test Redirect
```
URL Path: /redirect1
```

---

## 🛠️ Démarrage du Serveur

```bash
cd /home/aravelom/Project/current/Webserv
make
./webserv config/aravelom.conf.pl
```

Vérifie que le serveur écoute sur le bon port (par défaut 8080).

---

## 🔍 Debugging

### Le serveur ne répond pas?

1. Vérifie le serveur est lancé
2. Vérifie le port correct (écoute vraiment 8080?)
3. Essaie dans ton terminal:
   ```bash
   curl -v http://localhost:8080/
   ```

4. Regarde la console du navigateur (F12) pour erreurs CORS/réseau

### Pas de réponse dans le Testing Lab?

- Vérifie que "Host" et "Port" sont corrects
- Regarde le "Request Log" pour voir les erreurs
- Toast notifications affichent les erreurs détaillées

---

## 📊 Architecture

```
www/
├── homePage/
│   ├── index.html
│   └── assets/
│       ├── styles.css
│       └── script.js
│
├── testing-lab/
│   ├── index.html
│   └── assets/
│       ├── styles.css
│       └── script.js
│
└── README.md (ce fichier)
```

---

## 🎯 Cas d'Usage

### Tester GET
→ Raw HTTP Playground + GET method

### Upload fichiers
→ Upload Test section

### Debug CGI
→ CGI Scripts section

### Valider config serveur
→ Dashboard + Server Status

### Tester redirects
→ Redirect Tester

### Monitor toutes les requêtes
→ Logs section (terminal-like)

---

## 🚀 Production

Ces applications utilisent **Fetch API** (moderne et standard).

Elles n'envoient que des requêtes HTTP/1.1 réelles au serveur cible.

Pas de simulation - tout est réel. ✅
