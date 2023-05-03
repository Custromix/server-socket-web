const serverUrl = "http://127.0.0.1:28489";

fetch(serverUrl)
    .then(response => {
        if (!response.ok) {
            throw new Error(`HTTP error! Status: ${response.status}`);
        }
        return response.text();
    })
    .then(data => {
        console.log(data);
    })
    .catch(error => {
        console.error(`Fetch error: ${error}`);
    });