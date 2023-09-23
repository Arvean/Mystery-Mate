import './Menu.css';

function Menu() {
    const [isGameStarted, setIsGameStarted] = useState(false);
    const [isJoining, setIsJoining] = useState(false);


    const startNewGame = async () => {
        try {
            const response = await fetch("")
        } catch (error) {
            console.error('There was a problem with the fetch operation:', error);
        }
    }
}
