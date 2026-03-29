let capacity = 0;
const cache = new Map();

// 🔹 Set Capacity (Initialize Cache)
function setCapacity() {
    const input = document.getElementById("capacityInput").value;
    const parsed = parseInt(input);

    if (isNaN(parsed) || parsed <= 0) {
        alert("Capacity must be greater than 0");
        return;
    }

    capacity = parsed;
    cache.clear();
    updateUI();

    // Lock capacity after setting (like backend init)
    document.getElementById("capacityInput").disabled = true;

    alert("Cache initialized with capacity " + capacity);
}

// 🔹 PUT Operation
function put() {
    if (capacity === 0) {
        alert("Please set capacity first!");
        return;
    }

    const key = parseInt(document.getElementById("keyInput").value);
    const value = parseInt(document.getElementById("valueInput").value);

    if (isNaN(key) || isNaN(value)) {
        alert("Enter valid numeric key and value");
        return;
    }

    // If key exists → remove it (update position)
    if (cache.has(key)) {
        cache.delete(key);
    }

    // Insert as MRU
    cache.set(key, value);

    // Remove LRU if capacity exceeded
    if (cache.size > capacity) {
        const lruKey = cache.keys().next().value;
        cache.delete(lruKey);
    }

    updateUI();
}

// 🔹 GET Operation
function get() {
    if (capacity === 0) {
        alert("Please set capacity first!");
        return;
    }

    const key = parseInt(document.getElementById("keyInput").value);

    if (isNaN(key)) {
        alert("Enter a valid numeric key");
        return;
    }

    if (!cache.has(key)) {
        alert("Key not found");
        return;
    }

    const value = cache.get(key);

    // Move to MRU
    cache.delete(key);
    cache.set(key, value);

    updateUI();

    alert("Value: " + value);
}

// 🔹 Reset Cache (optional but useful)
function resetCache() {
    cache.clear();
    capacity = 0;

    document.getElementById("capacityInput").disabled = false;
    document.getElementById("capacityInput").value = "";

    updateUI();

    alert("Cache reset!");
}

// 🔹 Update UI (Cache Visualization)
function updateUI() {
    const container = document.getElementById("cacheContainer");
    container.innerHTML = "";

    // MRU at front → reverse order
    const entries = Array.from(cache.entries()).reverse();

    entries.forEach(([key, value], index) => {
        const box = document.createElement("div");
        box.className = "cache-box";

        // Show key:value
        box.innerText = `${key}:${value}`;

        // Highlight MRU
        if (index === 0) {
            box.style.backgroundColor = "#4CAF50";
            box.style.color = "white";
        }

        container.appendChild(box);
    });
}