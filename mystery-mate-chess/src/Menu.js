import './Menu.css';

const apiBaseUrl = 'https://mystery-mate-09f0bde019c8.herokuapp.com/game';

function Menu() {
    const [isGameStarted, setIsGameStarted] = useState(false);
    const [isJoining, setIsJoining] = useState(false);


    const startNewGame = async () => {
        try {
            const response = await fetch(apiBaseUrl + '/startNew', {
                method: 'POST',
                headers: {
                  'Content-Type': 'application/json',
                },
                body: JSON.stringify(horcruxeId),
            });

            if (!response.ok) throw new Error('Network response was not ok');
            const data = await response.json();
            console.log('Joined game:', data);

        } catch (error) {
            console.error('There was a problem with the fetch operation:', error);
        }
    }
}
