// KLang Documentation - Service Worker
// Version: 1.0.0

const CACHE_NAME = 'klang-docs-v1';
const urlsToCache = [
    '/',
    '/docs/web/index.html',
    '/docs/web/docs.html',
    '/docs/web/styles.css',
    '/docs/web/script.js',
    '/docs/web/README.md',
    'https://fonts.googleapis.com/css2?family=Inter:wght@300;400;500;600;700;800;900&family=JetBrains+Mono:wght@400;500;600;700&display=swap',
    'https://cdnjs.cloudflare.com/ajax/libs/highlight.js/11.9.0/styles/atom-one-dark.min.css',
    'https://cdnjs.cloudflare.com/ajax/libs/highlight.js/11.9.0/highlight.min.js',
    'https://cdn.jsdelivr.net/npm/marked/marked.min.js'
];

// Install event - cache resources
self.addEventListener('install', event => {
    console.log('[ServiceWorker] Installing...');
    
    event.waitUntil(
        caches.open(CACHE_NAME)
            .then(cache => {
                console.log('[ServiceWorker] Caching app shell');
                return cache.addAll(urlsToCache);
            })
            .then(() => {
                console.log('[ServiceWorker] Installed successfully');
                return self.skipWaiting();
            })
            .catch(error => {
                console.error('[ServiceWorker] Installation failed:', error);
            })
    );
});

// Activate event - clean up old caches
self.addEventListener('activate', event => {
    console.log('[ServiceWorker] Activating...');
    
    event.waitUntil(
        caches.keys().then(cacheNames => {
            return Promise.all(
                cacheNames.map(cacheName => {
                    if (cacheName !== CACHE_NAME) {
                        console.log('[ServiceWorker] Removing old cache:', cacheName);
                        return caches.delete(cacheName);
                    }
                })
            );
        }).then(() => {
            console.log('[ServiceWorker] Activated successfully');
            return self.clients.claim();
        })
    );
});

// Fetch event - serve from cache, fallback to network
self.addEventListener('fetch', event => {
    // Skip cross-origin requests
    if (!event.request.url.startsWith(self.location.origin) && 
        !event.request.url.includes('googleapis.com') &&
        !event.request.url.includes('cdnjs.cloudflare.com') &&
        !event.request.url.includes('jsdelivr.net')) {
        return;
    }
    
    event.respondWith(
        caches.match(event.request)
            .then(response => {
                // Return cached version or fetch from network
                if (response) {
                    console.log('[ServiceWorker] Serving from cache:', event.request.url);
                    return response;
                }
                
                console.log('[ServiceWorker] Fetching:', event.request.url);
                return fetch(event.request).then(response => {
                    // Don't cache non-successful responses
                    if (!response || response.status !== 200 || response.type === 'error') {
                        return response;
                    }
                    
                    // Clone the response
                    const responseToCache = response.clone();
                    
                    // Cache the fetched resource
                    caches.open(CACHE_NAME).then(cache => {
                        cache.put(event.request, responseToCache);
                    });
                    
                    return response;
                });
            })
            .catch(error => {
                console.error('[ServiceWorker] Fetch failed:', error);
                
                // Return offline page if available
                return caches.match('/offline.html');
            })
    );
});

// Background sync (for future use)
self.addEventListener('sync', event => {
    console.log('[ServiceWorker] Background sync:', event.tag);
    
    if (event.tag === 'sync-data') {
        event.waitUntil(
            // Perform background sync operations
            Promise.resolve()
        );
    }
});

// Push notifications (for future use)
self.addEventListener('push', event => {
    console.log('[ServiceWorker] Push notification received');
    
    const options = {
        body: event.data ? event.data.text() : 'New update available!',
        icon: '/icon-192.png',
        badge: '/badge-72.png',
        vibrate: [200, 100, 200],
        actions: [
            {action: 'explore', title: 'View', icon: '/icon-check.png'},
            {action: 'close', title: 'Close', icon: '/icon-x.png'}
        ]
    };
    
    event.waitUntil(
        self.registration.showNotification('KLang Documentation', options)
    );
});

// Notification click
self.addEventListener('notificationclick', event => {
    console.log('[ServiceWorker] Notification clicked:', event.action);
    
    event.notification.close();
    
    if (event.action === 'explore') {
        event.waitUntil(
            clients.openWindow('/')
        );
    }
});

// Message from client
self.addEventListener('message', event => {
    console.log('[ServiceWorker] Message received:', event.data);
    
    if (event.data.action === 'skipWaiting') {
        self.skipWaiting();
    }
    
    if (event.data.action === 'clearCache') {
        event.waitUntil(
            caches.keys().then(cacheNames => {
                return Promise.all(
                    cacheNames.map(cacheName => caches.delete(cacheName))
                );
            }).then(() => {
                event.ports[0].postMessage({success: true});
            })
        );
    }
});

console.log('[ServiceWorker] Loaded successfully');
