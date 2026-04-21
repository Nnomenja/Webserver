Nginx propose bien des directives de configuration pour gérer les timeouts de manière globale. Ces paramètres se placent généralement dans le bloc `http` du fichier `nginx.conf` pour s'appliquer à l'ensemble des serveurs virtuels (sites) qu'il gère .

Voici un tableau récapitulatif des principales directives de timeout à configurer au niveau global :

| Directive | Rôle | Contexte de configuration |
| :--- | :--- | :--- |
| **`client_header_timeout`** | Définit le délai pour recevoir l'en-tête complet de la requête du client . | `http`, `server`, `location` |
| **`client_body_timeout`** | Définit le délai entre deux réceptions successives du corps de la requête . | `http`, `server`, `location` |
| **`send_timeout`** | Définit le délai pour envoyer la réponse au client. Essentiel pour les fichiers volumineux . | `http`, `server`, `location` |
| **`keepalive_timeout`** | Définit la durée pendant laquelle une connexion persistante (keep-alive) reste ouverte sans activité . | `http`, `server`, `location` |
| **`proxy_connect_timeout`** | Délai pour établir la connexion avec un serveur backend (en amont) . | `http`, `server`, `location` |
| **`proxy_read_timeout`** | Délai d'attente d'une réponse de la part du serveur backend . | `http`, `server`, `location` |
| **`proxy_send_timeout`** | Délai pour envoyer une requête au serveur backend . | `http`, `server`, `location` |

### ⚙️ Comment configurer ces timeouts globalement

La configuration se fait dans le fichier principal de Nginx, généralement situé à `/etc/nginx/nginx.conf`. Pour appliquer un timeout à tous vos sites, placez les directives dans le bloc `http`.

Voici un exemple de configuration globale typique :

```nginx
http {
    # --- Timeouts côté client (s'applique à tous les sites) ---
    client_header_timeout 120s;   # Temps accordé au client pour envoyer l'en-tête
    client_body_timeout   120s;   # Temps accordé au client pour envoyer le corps
    send_timeout          300s;   # Important pour les téléchargements de gros fichiers
    keepalive_timeout     120s;   # Durée de vie des connexions persistantes

    # --- Timeouts pour le proxy (si Nginx agit comme reverse proxy) ---
    proxy_connect_timeout 600s;   # Délai de connexion au serveur backend
    proxy_read_timeout    600s;   # Délai d'attente d'une réponse du backend
    proxy_send_timeout    600s;   # Délai d'envoi au backend

    # Inclut la configuration des sites
    include /etc/nginx/conf.d/*.conf;
    include /etc/nginx/sites-enabled/*;
}
```