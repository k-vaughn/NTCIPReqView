document.addEventListener('DOMContentLoaded', function() {
    // Resizing columns
    const resizers = document.querySelectorAll('.resizer');
    let startX, startWidth, currentResizer;

    resizers.forEach(resizer => {
        resizer.addEventListener('mousedown', initResize);
    });

    function initResize(e) {
        currentResizer = e.target;
        startX = e.clientX;
        startWidth = currentResizer.parentElement.offsetWidth;
        document.addEventListener('mousemove', doResize);
        document.addEventListener('mouseup', stopResize);
    }

    function doResize(e) {
        currentResizer.parentElement.style.width = startWidth + (e.clientX - startX) + 'px';
    }

    function stopResize() {
        document.removeEventListener('mousemove', doResize);
        document.removeEventListener('mouseup', stopResize);
    }

    // Hiding/showing columns
    const checkboxes = document.querySelectorAll('.controls input[type="checkbox"]');
    checkboxes.forEach(checkbox => {
        checkbox.addEventListener('change', function() {
            const column = this.getAttribute('data-column');
            const table = document.querySelector('.resizable');
            const th = table.querySelectorAll('th')[column];
            const tds = table.querySelectorAll('tbody tr td:nth-child(' + (parseInt(column) + 1) + ')');
            if (this.checked) {
                th.style.display = '';
                tds.forEach(td => td.style.display = '');
            } else {
                th.style.display = 'none';
                tds.forEach(td => td.style.display = 'none');
            }
        });
    });
});